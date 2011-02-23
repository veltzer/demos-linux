#ifndef __kernel_helper_h
#define __kernel_helper_h

// do we want debug information ?!?
//#define DO_DEBUG
// TODO - the macros here could (and should?!?) be turned into inline functions...

#ifdef DO_DEBUG
#define DEBUG(fmt, args...) printk(KERN_DEBUG "DEBUG %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)
#else
#define DEBUG(fmt, args...)
#endif
// errors are always shown
#define ERROR(fmt, args...) printk(KERN_ERR "ERROR %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)

// prints are always shown
#define INFO(fmt, args...) printk(KERN_INFO "INFO %s %s %s %d: " fmt "\n", THIS_MODULE->name, __BASE_FILE__, __FUNCTION__, __LINE__, ## args)

static inline int memcheck(void *buf, char val, unsigned int size) {
	unsigned int i;
	char         *cbuf = (char *)buf;

	for (i = 0; i < size; i++) {
		if (cbuf[i] != val) {
			ERROR("value at %u is %c and not %c", i, cbuf[i], val);
			return(-EFAULT);
		}
	}
	return(0);
}

#endif // __kernel_helper_h
