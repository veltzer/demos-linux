#include <linux/module.h> // for MODULE_*, module_*
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC
#include <linux/fs.h> // for fops
#include <linux/device.h> // for class_create
#include <linux/slab.h> // for kzalloc
#include <asm/uaccess.h> // for copy_to_user, access_ok
#include <linux/cdev.h> // for cdev_*
#include <linux/sched.h> // for TASK_INTERRUPTIBLE and more constants
#include <linux/spinlock.h> // for spinlock_t and ops on it
#include <linux/wait.h> // for wait_queue_head_t and ops on it

#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A named pipe exercise");

/*
 * See the README.txt for the explanation of this exercise.
 *
 * TODO:
 */

static int pipes_count=8;
module_param(pipes_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipes_count, "How many pipes to create ?");
static int pipe_size=PAGE_SIZE;
module_param(pipe_size, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipe_size, "What is the pipe size ?");

// struct for each pipe
struct my_pipe {
	char* data;
	int size;
	int read_pos;
	int write_pos;
	wait_queue_head_t read_queue;
	wait_queue_head_t write_queue;
	spinlock_t lock;
	struct device* pipe_device;
};

static struct my_pipe* pipes;

static inline void init_pipe(struct my_pipe* pipe) {
	pipe->data=kzalloc(pipe_size,GFP_KERNEL);
	pipe->size=pipe_size;
	pipe->read_pos=0;
	pipe->write_pos=0;
	spin_lock_init(&pipe->lock);
	pipe->pipe_device=NULL;
	init_waitqueue_head(&pipe->read_queue);
	init_waitqueue_head(&pipe->write_queue);
}

static inline void del_pipe(struct my_pipe* pipe) {
	kfree(pipe->data);
}

static inline bool have_data(struct my_pipe* pipe) {
	return pipe->read_pos!=pipe->write_pos;
}

// return the empty room of a pipe
static inline int pipe_room(struct my_pipe* pipe) {
	if(pipe->read_pos<pipe->write_pos) {
		return pipe->size-pipe->write_pos+pipe->read_pos;
	} else {
		return pipe->read_pos-pipe->write_pos;
	}
}
// return the occupied room of a pipe
static inline int pipe_data(struct my_pipe* pipe) {
	if(pipe->read_pos<pipe->write_pos) {
		return pipe->write_pos-pipe->read_pos;
	} else {
		return pipe->size-pipe->read_pos+pipe->write_pos;
	}
}

static inline void pipe_lock(struct my_pipe* pipe) {
	spin_lock(&pipe->lock);
}
static inline void pipe_unlock(struct my_pipe* pipe) {
	spin_unlock(&pipe->lock);
}
static inline int pipe_wait_read(struct my_pipe* pipe) {
	return wait_event_interruptible(pipe->read_queue,1);
}
static inline int pipe_wait_write(struct my_pipe* pipe) {
	return wait_event_interruptible(pipe->write_queue,1);
}

// these are the actual operations

static int open_pipe(struct inode * inode, struct file * file) {
	// hide the minor number in the private_data of the file_struct
	file->private_data=(void*)iminor(inode);
	return 0;
}

static ssize_t read_pipe(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	struct my_pipe* pipe;
	int data,size_to_read,first_chunk,second_chunk;
	INFO("start");
	pipe=pipes+(int)file->private_data;
	// lets sleep while there is no data in the pipe 
	pipe_lock(pipe);
	data=pipe_data(pipe);
	while(data==0) {
		pipe_unlock(pipe);
		if((ret=pipe_wait_read(pipe))) {
			return ret;
		}
		pipe_lock(pipe);
		data=pipe_data(pipe);
	}
	// now data > 0
	size_to_read=smin(data,count);
	// copy_to_user data from the pipe
	if(pipe->read_pos<pipe->write_pos) {
		copy_to_user(buf,pipe->data+pipe->read_pos,size_to_read);
		pipe->read_pos+=size_to_read;
	} else {
		first_chunk=smin(size_to_read,pipe->size-pipe->read_pos);
		copy_to_user(buf,pipe->data+pipe->read_pos,first_chunk);
		pipe->read_pos+=first_chunk;
		if(first_chunk<size_to_read) {
			second_chunk=size_to_read-first_chunk;
			copy_to_user(buf+first_chunk,pipe->data,second_chunk);
			pipe->read_pos=second_chunk;
		}
	}
	pipe_unlock(pipe);
	*ppos+=size_to_read;
	// wake up the writers
	wake_up_all(&pipe->write_queue);
	return size_to_read;
}

static ssize_t write_pipe(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	struct my_pipe* pipe;
	int room, size_to_write,first_chunk,second_chunk,ret;
	INFO("start");
	pipe=pipes+(int)file->private_data;
	pipe_lock(pipe);
	// lets check if we have room in the pipe
	room=pipe_room(pipe);
	while(room==0) {
		pipe_unlock(pipe);
		if((ret=pipe_wait_write(pipe))) {
			return ret;
		}
		pipe_lock(pipe);
		room=pipe_room(pipe);
	}
	// now room > 0
	size_to_write=smin(room,count);
	// copy_from_user data from the pipe
	if(pipe->read_pos<pipe->write_pos) {
		first_chunk=smin(size_to_write,pipe->size-pipe->write_pos);
		copy_from_user(pipe->data+pipe->write_pos,buf,first_chunk);
		pipe->write_pos+=first_chunk;
		if(first_chunk<size_to_write) {
			second_chunk=size_to_write-first_chunk;
			copy_from_user(pipe->data,buf,second_chunk);
			pipe->write_pos=second_chunk;
		}
	} else {
		copy_from_user(pipe->data+pipe->write_pos,buf,size_to_write);
		pipe->write_pos+=size_to_write;
	}
	pipe_unlock(pipe);
	*ppos+=size_to_write;
	// wake up the readers
	wake_up_all(&pipe->read_queue);
	return size_to_write;
}

// this is the operations table
static const struct file_operations pipe_fops = {
	.open=open_pipe,
	.read=read_pipe,
	.write=write_pipe,
};

// this variable will store the class
static struct class *my_class;
// this variable will hold our cdev struct
static struct cdev cdev;
// this is the first dev_t allocated to us...
static dev_t first_dev;
// this is our first minor
static int first_minor=0;

static int pipe_init(void) {
	int err=0;
	int i;
	// allocate all pipes
	pipes=(struct my_pipe*)kmalloc(sizeof(struct my_pipe)*pipes_count,GFP_KERNEL);
	// initialize all pipes
	for(i=0;i<pipes_count;i++) {
		init_pipe(pipes+i);
	}
	// allocate our own range of devices
	if((err=alloc_chrdev_region(&first_dev, first_minor, pipes_count, THIS_MODULE->name))) {
		ERROR("cannot alloc_chrdev_region");
		goto err_final;
	}
	INFO("allocated the region");
	// add the cdev structure
	cdev_init(&cdev, &pipe_fops);
	if((err=cdev_add(&cdev, first_dev, pipes_count))) {
		ERROR("cannot cdev_add");
		goto err_dealloc;
	}
	INFO("added the cdev");
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		ERROR("failed in class_create");
		err=PTR_ERR(my_class);
		goto err_cdev_del;
	}
	INFO("created the class");
	for(i=0;i<pipes_count;i++) {
	// and now lets auto-create a /dev/ node
		pipes[i].pipe_device=device_create(my_class, NULL, MKDEV(MAJOR(first_dev),first_minor+i),NULL,"%s%d",THIS_MODULE->name,i);
		if(IS_ERR(pipes[i].pipe_device)) {
			ERROR("failed in device_create");
			err=PTR_ERR(pipes[i].pipe_device);
			goto err_class;
		}
	}
	INFO("created the device");
	INFO("loaded ok");
	return 0;
//err_device:
	device_destroy(my_class, first_dev);
err_class:
	class_destroy(my_class);
err_cdev_del:
	cdev_del(&cdev);
err_dealloc:
	unregister_chrdev_region(first_dev, pipes_count);
err_final:
	return err;
}

static void pipe_exit(void) {
	int i;
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class,MKDEV(MAJOR(first_dev),i));
	}
	class_destroy(my_class);
	cdev_del(&cdev);
	unregister_chrdev_region(first_dev, pipes_count);
	for(i=0;i<pipes_count;i++) {
		del_pipe(pipes+i);
	}
	INFO("unloaded ok");
}

module_init(pipe_init);
module_exit(pipe_exit);
