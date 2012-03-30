/*
 * procfs4.c - create a "file" in /proc
 * This program uses the seq_file library to manage the /proc file.
 */

#define DEBUG
#include<linux/module.h> // for the MODULE_*
#include<linux/printk.h> // for printk and the pr_* API
#include<linux/proc_fs.h> // Necessary because we use proc fs
#include<linux/seq_file.h> // for seq_file

#define PROC_NAME KBUILD_MODNAME

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("a module showing how to use the _seq /proc API");

/*
 * This function is called at the beginning of a sequence.
 * ie, when:
 * - the /proc file is read (first time)
 * - after the function stop (end of sequence)
 *
 */
static void* my_seq_start(struct seq_file *s, loff_t *pos) {
	pr_debug("my_seq_start %p, %Ld",s,*pos);
	/* beginning a new sequence ? */
	if ( *pos == 0 ) {
		/* yes => return a non null value to begin the sequence */
		int* p=(int*)kmalloc(sizeof(int),GFP_KERNEL);
		*p=0;
		return p;
	} else {
		/* no => it's the end of the sequence, return NULL to stop reading */
		return NULL;
	}
}

/*
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos) {
	int *p;
	pr_debug("my_seq_next %p, %p, %Ld",s,v,*pos);
	p=(int*)v;
	if(*p<10) {
		(*p)++;
		(*pos)++;
		return v;
	} else {
		return NULL;
	}
}

/*
 * This function is called at the end of a sequence
 */
static void my_seq_stop(struct seq_file *s, void *v) {
	pr_debug("my_seq_stop %p, %p",s,v);
	kfree(v);
}

/*
 * This function is called for each "step" of a sequence
 */
static int my_seq_show(struct seq_file *s, void *v) {
	int* p;
	pr_debug("my_seq_show %p, %p",s,v);
	p=(int*)v;
	seq_printf(s, "%d\n", *p);
	return 0;
}

/*
 * This structure gather "function" to manage the sequence
 */
static struct seq_operations my_seq_ops={
	.start=my_seq_start,
	.next=my_seq_next,
	.stop=my_seq_stop,
	.show=my_seq_show
};

/*
 * This function is called when the /proc file is open.
 */
static int my_open(struct inode *inode, struct file *file) {
	return seq_open(file,&my_seq_ops);
};

/*
 * This structure gathers "functions" that manage the /proc file
 */
static struct file_operations my_file_ops = {
	.owner=THIS_MODULE,
	.open=my_open,
	.read=seq_read,
	.llseek=seq_lseek,
	.release=seq_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void) {
	struct proc_dir_entry *entry;
	entry = create_proc_entry(PROC_NAME, 0, NULL);
	if (entry) {
		entry->proc_fops = &my_file_ops;
	}
	pr_info(KBUILD_MODNAME " loaded successfully");
	return 0;
}

/*
 * This function is called when the module is unloaded.
 */
void cleanup_module(void) {
	remove_proc_entry(PROC_NAME, NULL);
	pr_info(KBUILD_MODNAME " unloaded successfully");
}
