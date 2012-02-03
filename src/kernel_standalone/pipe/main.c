//#define DEBUG
#include <linux/module.h> // for MODULE_*, module_*
#include <linux/moduleparam.h> // for module_param, MODULE_PARM_DESC
#include <linux/fs.h> // for fops
#include <linux/device.h> // for class_create
#include <linux/slab.h> // for kzalloc
#include <linux/uaccess.h> // for copy_to_user, access_ok
#include <linux/cdev.h> // for cdev_*
#include <linux/sched.h> // for TASK_INTERRUPTIBLE and more constants
#include <linux/spinlock.h> // for spinlock_t and ops on it
#include <linux/wait.h> // for wait_queue_head_t and ops on it

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("A named pipe exercise");

#define DO_COPY
#define DO_SPINLOCK
//#define DO_MUTEX
#define DO_WAITQUEUE
#define DO_WAKEUP
//#define DO_SCHEDULE
//#define DO_NOTHING

/*
 * See exercise.txt for the description of this exercise.
 *
 * Notes:
 * - we protect the pipe here using a spinlock. Maybe a mutex would give better results
 *   on a single CPU. Test it out.
 * - Even though the pipe size is PAGE_SIZE (4096 on a i386) we can only store PAGE_SIZE-1
 *   bytes in the pipe. This is because if we stored 4096 bytes in the pipe we would not
 *   be able to distinguish between a full pipe and an empty one. This is the reason for the
 *   weird -1 in the 'pipe_room' function.
 * - the performace of this pipe as can be ascertained using the 'pipemeter' is much
 *   lower than the kernels own pipe as can be checked
 *   via 'cat /dev/zero | pipemeter > /dev/null' (I prepared a script for this).
 *
 * TODO:
 * - explain the difference in performance mentioned above and improve this example to
 *   give similar performance.
 *
 *
 * TOOLS: pipemeter
 */

static int pipes_count=8;
module_param(pipes_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipes_count, "How many pipes to create ?");
static int pipe_size=PAGE_SIZE*100;
module_param(pipe_size, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(pipe_size, "What is the pipe size ?");

// struct for each pipe
typedef struct _my_pipe_t {
	char* data;
	size_t size;
	size_t read_pos;
	size_t write_pos;
	wait_queue_head_t read_queue;
	wait_queue_head_t write_queue;
	#ifdef DO_SPINLOCK
	spinlock_t lock;
	#endif // DO_SPINLOCK
	struct device* pipe_device;
	#ifdef DO_MUTEX
	struct inode* inode;
	#endif // DO_MUTEX
	int readers;
	int writers;
} my_pipe_t;

static my_pipe_t* pipes=NULL;

// pipe constructur
static inline void pipe_ctor(my_pipe_t* pipe) {
	pipe->data=kzalloc(pipe_size,GFP_KERNEL);
	pipe->size=pipe_size;
	pipe->read_pos=0;
	pipe->write_pos=0;
#ifdef DO_SPINLOCK
	spin_lock_init(&pipe->lock);
#endif // DO_SPINLOCK
	pipe->pipe_device=NULL;
	init_waitqueue_head(&pipe->read_queue);
	init_waitqueue_head(&pipe->write_queue);
	pipe->readers=0;
	pipe->writers=0;
}

// pipe destructor
static inline void pipe_dtor(my_pipe_t* pipe) {
	kfree(pipe->data);
}

// do we have data in the pipe ?
static inline bool pipe_have_data(const my_pipe_t* pipe) {
	return pipe->read_pos!=pipe->write_pos;
}

// return the empty room of a pipe
static inline size_t pipe_room(const my_pipe_t* pipe) {
	if(pipe->read_pos<=pipe->write_pos) {
		return pipe->size-pipe->write_pos+pipe->read_pos-1;
	} else {
		return pipe->read_pos-pipe->write_pos-1;
	}
}
// return the occupied room of a pipe
static inline size_t pipe_data(const my_pipe_t* pipe) {
	if(pipe->read_pos<=pipe->write_pos) {
		return pipe->write_pos-pipe->read_pos;
	} else {
		return pipe->size-pipe->read_pos+pipe->write_pos;
	}
}

// lock the pipe spinlock
static inline void pipe_lock(my_pipe_t* const pipe) {
	#ifdef DO_SPINLOCK
	spin_lock(&pipe->lock);
	#endif // DO_SPINLOCK
	#ifdef DO_MUTEX
	mutex_lock(&pipe->inode->i_mutex);
	#endif // DO_MUTEX
}

// unlock the pipe spinlock
static inline void pipe_unlock(my_pipe_t* const pipe) {
	#ifdef DO_SPINLOCK
	spin_unlock(&pipe->lock);
	#endif // DO_SPINLOCK
	#ifdef DO_MUTEX
	mutex_unlock(&pipe->inode->i_mutex);
	#endif // DO_MUTEX
}

// wait on the pipes readers queue
static inline int pipe_wait_read(my_pipe_t* const pipe) {
	#ifdef DO_WAITQUEUE
	int ret;
	pipe_unlock(pipe);
	ret=wait_event_interruptible(pipe->read_queue,pipe_data(pipe)>0);
	pipe_lock(pipe);
	return ret;
	#endif // DO_WAITQUEUE
	#ifdef DO_SCHEDULE
	int ret;
	DEFINE_WAIT(wait);
	prepare_to_wait(&pipe->read_queue, &wait, TASK_INTERRUPTIBLE);
	pipe_unlock(pipe);
	schedule();
	if(signal_pending(current)) {
		ret=-ERESTARTSYS;
	} else {
		ret=0;
	}
	finish_wait(&pipe->read_queue, &wait);
	pipe_lock(pipe);
	return ret;
	#endif // DO_SHCEDULE
	#ifdef DO_NOTHING
	return 0;
	#endif // DO_NOTHING
}

// wait on the pipes writers queue
static inline int pipe_wait_write(my_pipe_t* const pipe) {
	#ifdef DO_WAITQUEUE
	int ret;
	pipe_unlock(pipe);
	ret=wait_event_interruptible(pipe->write_queue,pipe_room(pipe)>0);
	pipe_lock(pipe);
	return ret;
	#endif // DO_WAITQUEUE
	#ifdef DO_SCHEDULE
	int ret;
	DEFINE_WAIT(wait);
	prepare_to_wait(&pipe->write_queue, &wait, TASK_INTERRUPTIBLE);
	pipe_unlock(pipe);
	schedule();
	if(signal_pending(current)) {
		ret=-ERESTARTSYS;
	} else {
		ret=0;
	}
	finish_wait(&pipe->write_queue, &wait);
	pipe_lock(pipe);
	return ret;
	#endif // DO_SHCEDULE
	#ifdef DO_NOTHING
	return 0;
	#endif // DO_NOTHING
}

// wake up the readers
static inline void pipe_wake_readers(my_pipe_t* const pipe) {
	#ifdef DO_WAKEUP
	wake_up_all(&pipe->read_queue);
	#endif // DO_WAKEUP
}

// wake up the writers
static inline void pipe_wake_writers(my_pipe_t* const pipe) {
	#ifdef DO_WAKEUP
	wake_up_all(&pipe->write_queue);
	#endif // DO_WAKEUP
}

// read into the pipe
static inline int pipe_copy_from_user(my_pipe_t* const pipe,int count,const char** __user ubuf) {
	int ret;
	pr_debug("count is %d, read_pos is %d, write_pos is %d, size is %d",count,pipe->read_pos,pipe->write_pos,pipe->size);
	#ifdef DO_COPY
	ret=copy_from_user(pipe->data+pipe->write_pos,*ubuf,count);
	#else // DO_COPY
	ret=0;
	#endif // DO_COPY
	if(ret==0) {
		*ubuf+=count;
		pipe->write_pos+=count;
		//BUG_ON(pipe->write_pos>pipe->size);
		if(pipe->write_pos==pipe->size) {
			pipe->write_pos=0;
		}
	}
	return ret;
}

// read from the pipe
static inline int pipe_copy_to_user(my_pipe_t* const pipe,int count,char** __user ubuf) {
	int ret;
	pr_debug("count is %d, read_pos is %d, write_pos is %d, size is %d",count,pipe->read_pos,pipe->write_pos,pipe->size);
	#ifdef DO_COPY
	ret=copy_to_user(*ubuf,pipe->data+pipe->read_pos,count);
	#else // DO_COPY
	ret=0;
	#endif // DO_COPY
	if(ret==0) {
		*ubuf+=count;
		pipe->read_pos+=count;
		//BUG_ON(pipe->read_pos>pipe->size);
		if(pipe->read_pos==pipe->size) {
			pipe->read_pos=0;
		}
	}
	return ret;
}

// these are the actual operations

static int pipe_open(struct inode * inode, struct file * filp) {
	// hide the minor number in the private_data of the file_struct
	int minor=iminor(inode);
	my_pipe_t* pipe=pipes+minor;
#ifdef DO_MUTEX
	pipe->inode=inode;
#endif // DO_MUTEX
	if(filp->f_mode & FMODE_READ) {
		pipe->readers++;
	}
	if(filp->f_mode & FMODE_WRITE) {
		pipe->writers++;
	}
	filp->private_data=pipe;
	return 0;
}

static int pipe_release(struct inode* inode,struct file* filp) {
	my_pipe_t* pipe;
	pipe=(my_pipe_t*)(filp->private_data);
	if(filp->f_mode & FMODE_READ) {
		pipe->readers--;
	}
	if(filp->f_mode & FMODE_WRITE) {
		pipe->writers--;
	}
	return 0;
}

static ssize_t pipe_read(struct file * file, char __user * buf, size_t count, loff_t *ppos) {
	my_pipe_t* pipe;
	size_t data,work_size,first_chunk,second_chunk,ret;
	pr_debug("start");
	if (!access_ok(VERIFY_WRITE, buf, count))
		return -EFAULT;
	pipe=(my_pipe_t*)(file->private_data);
	// lets sleep while there is no data in the pipe 
	// why do we not just use the waitqueue condition? because we want to get
	// the pipe LOCKED with data
	pipe_lock(pipe);
	data=pipe_data(pipe);
	while(data==0 && pipe->writers>0) {
		if((ret=pipe_wait_read(pipe))) {
			pipe_unlock(pipe);
			return ret;
		}
		data=pipe_data(pipe);
	}
	pr_debug("data is %d",data);
	// EOF handling
	if(data==0 && pipe->writers==0) {
		pipe_unlock(pipe);
		return 0;
	}
	// now data > 0
	work_size=min(data,count);
	pr_debug("work_size is %d",work_size);
	// copy_to_user data from the pipe
	if(pipe->read_pos<=pipe->write_pos) {
		if((ret=pipe_copy_to_user(pipe,work_size,&buf))) {
			pipe_unlock(pipe);
			return ret;
		}
	} else {
		first_chunk=min(work_size,pipe->size-pipe->read_pos);
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

static ssize_t pipe_write(struct file * file, const char __user * buf, size_t count, loff_t *ppos) {
	my_pipe_t* pipe;
	size_t work_size,room,first_chunk,second_chunk,ret;
	pr_debug("start");
	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;
	pipe=(my_pipe_t*)(file->private_data);
	// lets check if we have room in the pipe
	// why do we not just use the waitqueue condition? because we want to get
	// the pipe LOCKED with data
	pipe_lock(pipe);
	room=pipe_room(pipe);
	while(room==0) {
		if((ret=pipe_wait_write(pipe))) {
			pipe_unlock(pipe);
			return ret;
		}
		room=pipe_room(pipe);
	}
	pr_debug("room is %d",room);
	// now room > 0
	work_size=min(room,count);
	pr_debug("work_size is %d",work_size);
	// copy_from_user data from the pipe
	if(pipe->read_pos<=pipe->write_pos) {
		first_chunk=min(work_size,pipe->size-pipe->write_pos);
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
	.open=pipe_open,
	.release=pipe_release,
	.read=pipe_read,
	.write=pipe_write,
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
	int ret;
	int i;
	// allocate all pipes
	pipes=(my_pipe_t*)kmalloc(sizeof(my_pipe_t)*pipes_count,GFP_KERNEL);
	if(IS_ERR(pipes)) {
		pr_err("kmalloc");
		ret=PTR_ERR(pipes);
		goto err_return;
	}
	// initialize all pipes
	for(i=0;i<pipes_count;i++) {
		pipe_ctor(pipes+i);
	}
	// allocate our own range of devices
	ret=alloc_chrdev_region(&first_dev, first_minor, pipes_count, THIS_MODULE->name);
	if(ret) {
		pr_err("cannot alloc_chrdev_region");
		goto err_final;
	}
	pr_debug("allocated the region");
	// add the cdev structure
	cdev_init(&cdev, &pipe_fops);
	ret=cdev_add(&cdev, first_dev, pipes_count);
	if(ret) {
		pr_err("cannot cdev_add");
		goto err_dealloc;
	}
	pr_debug("added the cdev");
	// this is creating a new class (/proc/devices)
	my_class=class_create(THIS_MODULE,THIS_MODULE->name);
	if(IS_ERR(my_class)) {
		pr_err("class_create");
		ret=PTR_ERR(my_class);
		goto err_cdev_del;
	}
	pr_debug("created the class");
	for(i=0;i<pipes_count;i++) {
		// and now lets auto-create a /dev/ node
		pipes[i].pipe_device=device_create(my_class, NULL, MKDEV(MAJOR(first_dev),first_minor+i),NULL,"%s%d",THIS_MODULE->name,i);
		if(IS_ERR(pipes[i].pipe_device)) {
			pr_err("device_create");
			ret=PTR_ERR(pipes[i].pipe_device);
			goto err_class;
		}
	}
	pr_info(KBUILD_MODNAME " loaded successfully");
	return 0;
	/*
err_device:
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class, MKDEV(MAJOR(first_dev),first_minor+i));
	}
	*/
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
err_return:
	return ret;
}

static void __exit pipe_exit(void) {
	int i;
	for(i=0;i<pipes_count;i++) {
		device_destroy(my_class,MKDEV(MAJOR(first_dev),MINOR(first_dev)+i));
	}
	class_destroy(my_class);
	cdev_del(&cdev);
	unregister_chrdev_region(first_dev, pipes_count);
	for(i=0;i<pipes_count;i++) {
		pipe_dtor(pipes+i);
	}
	kfree(pipes);
	pr_info(KBUILD_MODNAME " unloaded successfully");
}

module_init(pipe_init);
module_exit(pipe_exit);
