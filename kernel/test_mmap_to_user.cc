#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "us_helper.hh"
#include "shared.h"

/*
 *      This is a test for kernel driven mmap.
 *
 *      The test:
 *      - opens the file.
 *      - asks (via ioctl) to mmap to it's space.
 *      - closes the file.
 */

//const bool do_single=true;
//const bool do_stress=false;
const bool do_single = false;
const bool do_stress = true;
const unsigned int count = 10000;

int main(int argc, char **argv, char **envp) {
	// file to be used
	const char *filename = "/dev/demo";
	// file descriptor
	int d = -1;
	// user space pointer
	void *ptr = NULL;

	//klog_clear();

	SC(d = open(filename, O_RDWR));
	//printproc("demo");
	//klog_show_clear();
	waitkey(NULL);

	if (do_single) {
		// the size of data that we need
		const unsigned int size = 1000000;
		SCPE(ptr = (void *)ioctl(d, IOCTL_DEMO_MAP, size), "map to user");
		//printproc("demo");
		//klog_show_clear();
		waitkey(NULL);

		INFO("trying to write on the buffer");
		memset(ptr, 'a', size);
		// get buffer from user space
		SC(ioctl(d, IOCTL_DEMO_READ, 'a'));
		SC(ioctl(d, IOCTL_DEMO_WRITE, 'a' + 1));
		memcheck(ptr, 'a' + 1, size);

		SC(ioctl(d, IOCTL_DEMO_UNMAP, NULL));
		//printproc("demo");
		//klog_show_clear();
		waitkey(NULL);
	}
	if (do_stress) {
		do_prog_init();
		for (unsigned int i = 0; i < count; i++) {
			do_prog(i, 10, count);
			unsigned int size = (i % 1000 + 1) * 4096;
			char c = i % 20 + 'a';
			scp(ptr = (void *)ioctl(d, IOCTL_DEMO_MAP, size));
			// set the buffer to c
			memset(ptr, c, size);
			// get buffer from user space
			sc(ioctl(d, IOCTL_DEMO_READ, c));
			// ask the kernel to write c+1
			sc(ioctl(d, IOCTL_DEMO_WRITE, c + 1));
			// check that the buffer has the right data
			memcheck(ptr, c + 1, size);
			// get ridd of the in kernel buffer
			sc(ioctl(d, IOCTL_DEMO_UNMAP, NULL));
		}
		do_prog_finish();
	}

	SC(close(d));
	//printproc("demo");
	//klog_show_clear();
	waitkey(NULL);

	return(0);
}
