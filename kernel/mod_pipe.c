//#define DEBUG
#include<linux/module.h>		/* Module API */
#include<linux/moduleparam.h>		/* Module parameters API */
#include<linux/kernel.h>		/* printk() */
#include<linux/slab.h>			/* Memory alloc functions */
#include<linux/fs.h>			/* File operations */
#include<linux/errno.h>			/* Error codes */
#include<linux/interrupt.h>		/* Interrupts */
#include<linux/proc_fs.h>		/* Proc file */
#include<linux/cdev.h>			/* Character device */
#include<linux/sched.h>			/* TASK_* definitions */
#include<asm/uaccess.h>			/* User space access */

//#define DO_DEBUG
#include "kernel_helper.h"

/*
 * pipe.c
 * An exercise in making a pipe.
 */

MODULE_AUTHOR("Mark Veltzer");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A named pipe example");

/*
 * You are required to make a pipe like device.
 * The device will be exposed to user space via /dev/pipe[n].
 * A parameter to the module is how many pipes to create (8 by default).
 * The pipe size is also a parameter (4096 by default).
 *
 * Proc support:
 * provice /proc support to debug your driver. A file in /proc/drive/pipe should
 * be exposed that when printed will show all pipes status.
 *
 * TODO:
 * - devices are not created automagically - investigate.
 */

// device numbers assigned dynamically to us
static dev_t pipe_dev;
// character device driver strucutre
static struct cdev* pipe_cdev;
// do we want to debug this driver ?
static unsigned int debug_param=0;
// what is the pipe size
static unsigned int pipe_size_param=4096;
// how many pipes do we have ?
static unsigned int pipe_count_param=8;
// the /proc entry with which you can view the size of the pipe.
static const char* proc_filename="driver/pipe";
// the wait queue we will use to put processes to sleep on and wake them up...

// synchronization aids
// flag used to signal wakeup
//static unsigned int flag=0;
//DECLARE_WAIT_QUEUE_HEAD(pipe_read_wq);
//DECLARE_WAIT_QUEUE_HEAD(pipe_write_wq);

// module paramters
// The permissions are set up the way they are to allow everyone to see
// them and no one to change them (not even USR which is root since we
// do not handle changes to the number of pipes or sizes of pipes well at
// run time. Debug could maybe have been allowed to be changed but who
// cares anyway?
module_param(debug_param,uint,S_IRUSR | S_IRGRP | S_IROTH);
module_param(pipe_size_param,uint,S_IRUSR | S_IRGRP | S_IROTH);
module_param(pipe_count_param,uint,S_IRUSR | S_IRGRP | S_IROTH);

/*
 * Open the device. Optional.
 */
int pipe_open(struct inode* inode,struct file* filp) {
	PR_DEBUG("device is open");
	return 0;
}

/*
 * Release the device. Optional as well.
 */
int pipe_release(struct inode* inode,struct file* filp) {
	PR_DEBUG("device is released");
	return 0;
}

/*
 * Read from the pipe. This does the real work. This method always goes to sleep
 * until woken up by the interrupt handler. When it is woken up it copied the data from the
 * clip board. There is no real locking on the pipe status which is a problem. Fix this
 * sometime.
 */
ssize_t pipe_read(struct file* filp,char* user_buf,size_t count,loff_t* offset) {
	PR_DEBUG("read");
	return(-EFAULT);
}

/*
 * Writing to the pipe. This simply copied the data from user space into the in-kernel buffer
 * using copy_from_user. If the pipe is full we do not return 0 since this will cause the user
 * process to think that it had reached end of file. Instead, we put it to sleep and wait to be
 * woken up by a reader that has read from the pipe. Then we check the size again since the
 * pipe has chanced. Once we can write any positive amount we do the write and return the
 * number of bytes written to the user. Why don't we loop until we write the entire buffer of
 * the user (count bytes)? Well, we could do it but then we would not comply with the standard
 * UNIX semantics for write that state that we should write what we can NOW and return the number
 * of bytes written. 
 *
 * A simpler approach would have been to just see if we have at least count bytes free in the
 * buffer and return an error if that is not the case. This is too simplistic and would break
 * lots of prefectly legal user space programs.
 */
ssize_t pipe_write(struct file* filp,const char* user_buf,size_t count,loff_t* offset) {
	PR_DEBUG("read");
	return(-EFAULT);
}

/*
 * proc file callback
 */

int pipe_proc_reader(char* page,char** start,off_t off,int count,int* eof,void* data) {
	int len=0;
	len+=sprintf(page+len,"Hello world! debug param=%d\n",debug_param);
	if(len<=off+count)
		*eof=1;
	*start=page+off;
	len-=off;
	if(len>count) len=count;
	if(len<0) len=0;
	return len;
}

/* our file operations structure that gathers all the ops */

struct file_operations pipe_fops={
	.open=pipe_open,
	.release=pipe_release,
	.read=pipe_read,
	.write=pipe_write
};

/*
 * Module initialisation. 
 */
static int pipe_init(void) {
	int ret;
	struct proc_dir_entry* pipe_proc_file;

	pipe_proc_file=create_proc_entry(proc_filename,0,NULL);
	if(IS_ERR(pipe_proc_file)) {
		PR_ERROR("create_proc_entry");
		ret=PTR_ERR(pipe_proc_file);
		goto error_any;
	}
	pipe_proc_file->read_proc=pipe_proc_reader;

	ret=alloc_chrdev_region(&pipe_dev,0,pipe_count_param,"pipe");
	if(ret) {
		PR_ERROR("alloc_chrdev_region");
		goto error_after_proc;
	}

	pipe_cdev=cdev_alloc();
	if(IS_ERR(pipe_cdev)) {
		PR_ERROR("cdev_alloc");
		ret=PTR_ERR(pipe_cdev);
		goto error_after_alloc_chrdev;
	}
	pipe_cdev->ops=&pipe_fops;
	pipe_cdev->owner=THIS_MODULE;

	ret=cdev_add(pipe_cdev,pipe_dev,pipe_count_param);
	if(ret) {
		PR_ERROR("cdev_add");
		/*
		 * We put the kfree here since other error paths will only need to do
		 * cdev_del. The problem is that cdev_del IS NOT the exact anti function
		 * of cdev_add as it does the extra free. How do you find this ? The hard way
		 * by having your machine crash on you when you do cdev_del followed by kfree.
		 */
		kfree(pipe_cdev);
		goto error_after_alloc_chrdev;
	}

	PR_INFO("pipe loaded sucessfuly.");
	return 0;

//error_after_cdev_add:
//	cdev_del(pipe_cdev);
error_after_alloc_chrdev:
	unregister_chrdev_region(pipe_dev,pipe_count_param);
error_after_proc:
	remove_proc_entry(proc_filename,NULL);
error_any:
	return ret;
}

static void pipe_cleanup(void) {
	cdev_del(pipe_cdev);
	unregister_chrdev_region(pipe_dev,pipe_count_param);
	remove_proc_entry(proc_filename,NULL);
	PR_INFO("pipe unloaded succefully.");
}

module_init(pipe_init);
module_exit(pipe_cleanup);
