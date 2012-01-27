//#define DEBUG
#include <linux/module.h> // for MODULE_*
#include <linux/fs.h> // for fops
#include <linux/delay.h> // for udelay
#include <linux/cpufreq.h> // for cpufreq_*

#include "shared.h"

//#define DO_DEBUG
#include "kernel_helper.h" // our own helper

/*
 *	This driver shows how various timing delays are done in the kernel
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Modules for testing timing");

// static data
static struct device* my_device;

// now the functions
static void long_code(unsigned long mic) {
	/*
	 * int i;
	 * for(i=0;i<10000000;i++) {
	 *	void* ptr=kmalloc(1000,GFP_KERNEL);
	 *	kfree(ptr);
	 * }
	 */
	// sleep for 1 second
	//msleep(mil);
	//ssleep(1);
	udelay(mic);
}


/*
 * This is the ioctl implementation.
 */
// cycles_t is actually unsigned long long (look at arch/x86/include/asm/tsc.h).
static long kern_unlocked_ioctll(struct file *filp, unsigned int cmd, unsigned long arg) {
	// for register measurements...
	cycles_t curreg, cnt1, cnt2;
	unsigned long cdiff, crmil, crmic, crmic2;
	// for jiffies measurements...
	unsigned long j1, j2, jdiff, jmil, jmic;
	unsigned int freq;

	PR_DEBUG("start");
	//char str[256];
	//PR_DEBUG(str,"start");
	switch (cmd) {
	case IOCTL_TIMING_CLOCK:
		/* this shows how to work with the x86 counters */
		curreg = get_cycles();
		PR_INFO("get_cycles: %llu\n", curreg);
		// getting the cpufreq for cpu0
		// I used the quick version under the assumption that the
		// frequency doesn't change. If this assumption is not
		// correct and the cpu scales for some reason you need
		// to use 'cpufreq_get'.
		freq = cpufreq_quick_get(0);
		PR_INFO("cpufreq_quick_get: %i\n", freq);
		break;

	case IOCTL_TIMING_TSC:
		/* this is how to measure the speed of some code using counters */
		freq = cpufreq_quick_get(0);
		cnt1 = get_cycles();
		long_code(arg);
		cnt2 = get_cycles();
		cdiff = cnt2 - cnt1;
		crmil = cdiff / freq;
		crmic = crmil * 1000;
		crmic2 = (cdiff * 1000) / freq;
		PR_INFO("cnt1: %llu\n", cnt1);
		PR_INFO("cnt2: %llu\n", cnt2);
		PR_INFO("cdiff: %lu\n", cdiff);
		PR_INFO("crmil: %lu\n", crmil);
		PR_INFO("crmic: %lu\n", crmic);
		PR_INFO("crmic2: %lu\n", crmic2);
		break;

	case IOCTL_TIMING_JIFFIES:

		/*
		 *	This shows how to work with jiffies...
		 *	It will demostrate the most important attribute of jiffies and that
		 *	is that jiffies DO NOT change
		 */
		j1 = jiffies;
		long_code(arg);
		j2 = jiffies;
		jdiff = (j2 - j1) * 1000;
		//jmic=do_div(jdiff,HZ);
		jmil = jdiff / HZ;
		jmic = jmil * 1000;
		PR_INFO("j1 is %lu", j1);
		PR_INFO("j2 is %lu", j2);
		PR_INFO("jdiff is %lu", jdiff);
		PR_INFO("HZ is %d", HZ);
		PR_INFO("jmil is %lu", jmil);
		PR_INFO("jmic is %lu", jmic);
		break;

	case IOCTL_TIMING_EMPTY:
		/*
		 *	This syscall does nothing on purpose to enable timing
		 *	from user space
		 */
		//udelay(24);
		break;
	}
	return(0);
}


/*
 * The file operations structure.
 */
static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = kern_unlocked_ioctll,
};

#include "device.inc"
