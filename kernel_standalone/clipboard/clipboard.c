/*
 * clipboard.c
 * Originally from Alessandro Rubini Linux Device Driver book.
 * Much messed with by Gilad Ben-Yossef.
 * Fixed up by Mark Veltzer for newer kernels and added features.
 */
#include <linux/module.h>		/* Module API */
#include <linux/moduleparam.h>		/* Module parameters API */
#include <linux/kernel.h>		/* printk() */
#include <linux/slab.h>			/* Memory alloc functions */
#include <linux/fs.h>			/* File operations */
#include <linux/errno.h>		/* Error codes */
#include <linux/interrupt.h>		/* Interrupts */
#include <linux/proc_fs.h>		/* Proc file */
#include <linux/cdev.h>			/* Character device */
#include <linux/sched.h>		/* TASK_* definitions */
#include <asm/uaccess.h>		/* User space access */

MODULE_AUTHOR("Mark Veltzer <mark.veltzer@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A proc/interrupt/clipboard example");

static dev_t clipboard_dev;
static struct cdev *clipboard_cdev;
static char *clipboard=NULL;
static unsigned int flag=0;
unsigned int debug_param=0;
const unsigned int CLIPBOARD_SIZE=4096;
const unsigned int CLIPBOARD_COUNT=1;
const unsigned int MOUSE_INT=12;
const char* proc_filename="driver/clipboard";

module_param (debug_param, uint, S_IRWXU);

DECLARE_WAIT_QUEUE_HEAD(clipboard_wq);

/*
 * Open the device. Optional.
 */
int clipboard_open (struct inode *inode, struct file *filp) {
	printk(KERN_DEBUG "device is open\n");
	return 0;
}

/*
 * Release the device. Optional as well.
 */
int clipboard_release (struct inode *inode, struct file *filp) {
	printk(KERN_DEBUG "device is released\n");
	return 0;
}

/*
 * Read from the clipboard. This does the real work. This method always goes to sleep
 * until woken up by the interrupt handler. When it is woken up it copied the data from the
 * clip board. There is no real locking on the clipboard status which is a problem. Fix this
 * sometime.
 */
ssize_t clipboard_read (struct file * filp, char *user_buf, size_t count, loff_t * offset) {
	int remaining_bytes;
	/* Number of bytes left to read in the open file */
	remaining_bytes=min(CLIPBOARD_SIZE-(*offset),(loff_t)count);
	if (remaining_bytes == 0) {
		/* All read, returning 0 (End Of File) */
		return 0;
	}
	/* lets go to sleep until an intterupt occurs in which case we will have data */
	flag=0;
	wait_event_interruptible(clipboard_wq, flag);
	if(copy_to_user(
		user_buf /* to */,
		*offset+clipboard /* from */,
		remaining_bytes
	)) {
		return -EFAULT;
	} else {
		/* Increase the position in the open file */
		*offset += remaining_bytes;
		return remaining_bytes;
	}
}

/*
 * Writing to the clipboard. This simply copied the data from user space into the in-kernel buffer
 * using copy_from_user. If the clipboard does not have enough space for the data we return an
 * IO error. We could have handled this more gracefully and copied whatever we could.
 */
ssize_t clipboard_write (struct file * filp, const char *user_buf, size_t count, loff_t * offset) {
	/* Number of bytes not written yet in the device */
	int remaining_bytes=CLIPBOARD_SIZE - (*offset);
	if (count > remaining_bytes) {
		/* Can't write beyond the end of the device */
		return -EIO;
	}
	if (copy_from_user(*offset+clipboard /* to */, user_buf /* from */, count)) {
		return -EFAULT;
	} else {
		/* Increase the position in the open file */
		*offset += count;
		return count;
	}
}

/*
 * proc file callback
 */

int clipboard_proc_reader(char * page, char **start, off_t off, int count, int * eof, void * data) {
	int len=0;
	len += sprintf(page + len, "Hello world! debug param=%d\n", debug_param);
	if (len <= off+count)
		*eof=1;
	*start=page + off;
	len -= off;
	if (len > count) len=count;
	if (len < 0) len=0;
	return len;
}

/*
 * Interrupt callback
 */

irqreturn_t clipboard_int_handler(int irq, void * dev) {

	flag=1;
	mb();
	wake_up_interruptible(&clipboard_wq);

	return IRQ_HANDLED;
}

/* our file operations structure that gathers all the ops */

struct file_operations clipboard_fops={
	.open=clipboard_open,
	.release=clipboard_release,
	.read=clipboard_read,
	.write=clipboard_write
};
/*
 * Module housekeeping.
 */
static int clipboard_init (void) {
	int ret=0;
	struct proc_dir_entry * clipboard_proc_file;

	clipboard=kmalloc (CLIPBOARD_SIZE, GFP_KERNEL);
	if (!clipboard) {
		ret=-ENOMEM;
		goto any_error;
	}
	memset(clipboard,0,CLIPBOARD_SIZE);

	clipboard_proc_file=create_proc_entry(proc_filename, 0, NULL);
	if(!clipboard_proc_file) {
		ret=-ENOMEM;
		goto error_after_alloc;
	}
	clipboard_proc_file->read_proc=clipboard_proc_reader;

	if (alloc_chrdev_region(&clipboard_dev, 0, CLIPBOARD_COUNT, "clipboard")) {
		ret=-ENODEV;
		goto error_after_proc;
	}

	clipboard_cdev=cdev_alloc();
	if (!clipboard_cdev) {
		ret=-ENOMEM;
		goto error_after_region;
	}
	clipboard_cdev->ops=&clipboard_fops;
	clipboard_cdev->owner=THIS_MODULE;

	if (cdev_add(clipboard_cdev, clipboard_dev,CLIPBOARD_COUNT)) {
		ret=-ENODEV;
		/* Only if we allocated a cdev but did not register do we
		* we need to kfree it. In any other case cdev_del is enough */
		kfree(clipboard_cdev);
		goto error_after_region;
	}

	ret=request_irq(MOUSE_INT, clipboard_int_handler, IRQF_SHARED, "clipboard", &clipboard);
	if(ret) goto error_after_register;

	printk (KERN_INFO "clipboard loaded sucessfuly.\n");
	return 0;

error_after_register:
	cdev_del(clipboard_cdev);
error_after_region:
	unregister_chrdev_region(clipboard_dev, CLIPBOARD_COUNT);
error_after_proc:
	remove_proc_entry(proc_filename, NULL);
error_after_alloc:
	kfree(clipboard);
any_error:
	return ret;
}

static void clipboard_cleanup(void) {
	remove_proc_entry(proc_filename, NULL);
	free_irq(MOUSE_INT, &clipboard);
	cdev_del(clipboard_cdev);
	unregister_chrdev_region(clipboard_dev,CLIPBOARD_COUNT);
	kfree(clipboard);
	printk(KERN_INFO "clipboard unloaded succefully.\n");
}

module_init (clipboard_init);
module_exit (clipboard_cleanup);
