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

#define DO_DEBUG
#include "kernel_helper.h" // our own helper

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A named pipe exercise");

//#define DO_COPY

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

static struct my_pipe* pipes=NULL;

// pipe constructur
static inline void pipe_ctor(struct my_pipe* pipe) {
	pipe->data=kzalloc(pipe_size,GFP_KERNEL);
	pipe->size=pipe_size;
	pipe->read_pos=0;
	pipe->write_pos=0;
	spin_lock_init(&pipe->lock);
	pipe->pipe_device=NULL;
	init_waitqueue_head(&pipe->read_queue);
	init_waitqueue_head(&pipe->write_queue);
}

// pipe destructor
static inline void pipe_dtor(struct my_pipe* pipe) {
	kfree(pipe->data);
}

// do we have data in the pipe ?
static inline bool pipe_have_data(const struct my_pipe* pipe) {
	return pipe->read_pos!=pipe->write_pos;
}

// return the empty room of a pipe
static inline int pipe_room(const struct my_pipe* pipe) {
	if(pipe->read_pos<pipe->write_pos) {
		return pipe->size-pipe->write_pos+pipe->read_pos-1;
	} else {
		if(pipe->read_pos==pipe->write_pos) {
			return pipe->size-1;
		} else {
			return pipe->read_pos-pipe->write_pos-1;
		}
	}
}
// return the occupied room of a pipe
static inline int pipe_data(const struct my_pipe* pipe) {
	if(pipe->read_pos<pipe->write_pos) {
		return pipe->write_pos-pipe->read_pos;
	} else {
		if(pipe->read_pos==pipe->write_pos) {
			return 0;
		} else {
			return pipe->size-pipe->read_pos+pipe->write_pos-1;
		}
	}
}

// lock the pipe spinlock
static inline void pipe_lock(struct my_pipe* pipe) {
	spin_lock(&pipe->lock);
}

// unlock the pipe spinlock
static inline void pipe_unlock(struct my_pipe* pipe) {
	spin_unlock(&pipe->lock);
}

// wait on the pipes readers queue
static inline int pipe_wait_read(struct my_pipe* pipe) {
	return wait_event_interruptible(pipe->read_queue,1);
}

// wait on the pipes writers queue
static inline int pipe_wait_write(struct my_pipe* pipe) {
	return wait_event_interruptible(pipe->write_queue,1);
}

// wake up the readers
static inline void pipe_wake_readers(struct my_pipe* pipe) {
	wake_up_all(&pipe->read_queue);
}

// wake up the writers
static inline void pipe_wake_writers(struct my_pipe* pipe) {
	wake_up_all(&pipe->write_queue);
}

// read into the pipe
static inline int pipe_copy_from_user(struct my_pipe* pipe,int size,const char** __user ubuf) {
	#ifdef DO_COPY
	int ret=copy_from_user(pipe->data+pipe->read_pos,*ubuf,size);
	#else // DO_COPY
	int ret=0;
	INFO("size is %d, read_pos is %d, write_pos is %d",size,pipe->read_pos,pipe->write_pos);
	#endif // DO_COPY
	if(ret==0) {
		pipe->read_pos+=size;
		*ubuf+=size;
		if(pipe->read_pos==pipe->size) {
			pipe->read_pos=0;
		}
	}
	return ret;
}

// read from the pipe
static inline int pipe_copy_to_user(struct my_pipe* pipe,int size,char** __user ubuf) {
	#ifdef DO_COPY
	int ret=copy_to_user(ubuf,pipe->data+pipe->write_pos,size);
	#else // DO_COPY
	int ret=0;
	INFO("size is %d, read_pos is %d, write_pos is %d",size,pipe->read_pos,pipe->write_pos);
	#endif // DO_COPY
	if(ret==0) {
		pipe->write_pos+=size;
		*ubuf+=size;
		if(pipe->write_pos==pipe->size) {
			pipe->write_pos=0;
		}
	}
	return ret;
}

// these are the actual operations

static int open_pipe(struct inode * inode, struct file * file) {
	// hide the minor number in the private_data of the file_struct
	file->private_data=(void*)iminor(inode);
	return 0;
}

static ssize_t read_pipe(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	struct my_pipe* pipe;
	int data,work_size,first_chunk,second_chunk,minor,ret;
	DEBUG("start");
	minor=(int)(file->private_data);
	pipe=pipes+minor;
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
	DEBUG("data is %d",data);
	// now data > 0
	work_size=smin(data,count);
	DEBUG("work_size is %d",work_size);
	// copy_to_user data from the pipe
	if(pipe->read_pos<pipe->write_pos) {
		if((ret=pipe_copy_to_user(pipe,work_size,&buf))) {
			pipe_unlock(pipe);
			return ret;
		}
	} else {
		first_chunk=smin(work_size,pipe->size-pipe->read_pos);
		if((ret=pipe_copy_to_user(pipe,first_chunk,&buf))) {
			pipe_unlock(pipe);
			return ret;
		}
		if(first_chunk<work_size) {
			second_chunk=work_size-first_chunk;
			if((ret=pipe_copy_to_user(pipe,second_chunk,&buf))) {
				pipe_unlock(pipe);
				return ret;
			}
		}
	}
	pipe_unlock(pipe);
	*ppos+=work_size;
	// wake up the writers
	pipe_wake_writers(pipe);
	return work_size;
}

static ssize_t write_pipe(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	struct my_pipe* pipe;
	int room,work_size,first_chunk,second_chunk,minor,ret;
	DEBUG("start");
	minor=(int)(file->private_data);
	pipe=pipes+minor;
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
	DEBUG("room is %d",room);
	// now room > 0
	work_size=smin(room,count);
	DEBUG("work_size is %d",work_size);
	// copy_from_user data from the pipe
	if(pipe->read_pos<pipe->write_pos) {
		first_chunk=smin(work_size,pipe->size-pipe->write_pos);
		if((ret=pipe_copy_from_user(pipe,first_chunk,&buf))) {
			pipe_unlock(pipe);
			return ret;
		}
		if(first_chunk<work_size) {
			second_chunk=work_size-first_chunk;
			if((ret=pipe_copy_from_user(pipe,second_chunk,&buf))) {
				pipe_unlock(pipe);
				return ret;
			}
		}
	} else {
		if((ret=pipe_copy_from_user(pipe,work_size,&buf))) {
			pipe_unlock(pipe);
			return ret;
		}
	}
	pipe_unlock(pipe);
	*ppos+=work_size;
	// wake up the readers
	pipe_wake_readers(pipe);
	return work_size;
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

static int __init pipe_init(void) {
	int err=0;
	int i;
	INFO("started loading...");
	// allocate all pipes
	pipes=(struct my_pipe*)kmalloc(sizeof(struct my_pipe)*pipes_count,GFP_KERNEL);
	// initialize all pipes
	for(i=0;i<pipes_count;i++) {
		pipe_ctor(pipes+i);
	}
	// allocate our own range of devices
	if((err=alloc_chrdev_region(&first_dev, first_minor, pipes_count, THIS_MODULE->name))) {
		ERROR("cannot alloc_chrdev_region");
		goto err_final;
	}
	DEBUG("allocated the region");
	// add the cdev structure
	cdev_init(&cdev, &pipe_fops);
	if((err=cdev_add(&cdev, first_dev, pipes_count))) {
		ERROR("cannot cdev_add");
		goto err_dealloc;
	}
	DEBUG("added the cdev");
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		ERROR("failed in class_create");
		err=PTR_ERR(my_class);
		goto err_cdev_del;
	}
	DEBUG("created the class");
	for(i=0;i<pipes_count;i++) {
	// and now lets auto-create a /dev/ node
		pipes[i].pipe_device=device_create(my_class, NULL, MKDEV(MAJOR(first_dev),first_minor+i),NULL,"%s%d",THIS_MODULE->name,i);
		if(IS_ERR(pipes[i].pipe_device)) {
			ERROR("failed in device_create");
			err=PTR_ERR(pipes[i].pipe_device);
			goto err_class;
		}
	}
	DEBUG("created the device");
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
	for(i=0;i<pipes_count;i++) {
		pipe_dtor(pipes+i);
	}
	kfree(pipes);
	return err;
}

static void __exit pipe_exit(void) {
	int i;
	INFO("started unloading...");
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class,MKDEV(MAJOR(first_dev),i));
	}
	class_destroy(my_class);
	cdev_del(&cdev);
	unregister_chrdev_region(first_dev, pipes_count);
	for(i=0;i<pipes_count;i++) {
		pipe_dtor(pipes+i);
	}
	kfree(pipes);
	INFO("unloaded ok");
}

module_init(pipe_init);
module_exit(pipe_exit);
