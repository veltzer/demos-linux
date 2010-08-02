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
#include "ioctls.h"

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

	SCIE(d = open(filename, O_RDWR), "open");
	//printproc("demo");
	//klog_show_clear();
	waitkey();

	if (do_single) {
		// the size of data that we need
		const unsigned int size = 1000000;
		SCPE(ptr = (void *)ioctl(d, IOCTL_DEMO_MAP, size), "map to user");
		//printproc("demo");
		//klog_show_clear();
		waitkey();

		INFO("trying to write on the buffer");
		memset(ptr, 'a', size);
		// get buffer from user space
		SCIE(ioctl(d, IOCTL_DEMO_READ, 'a'), "read in kernel");
		SCIE(ioctl(d, IOCTL_DEMO_WRITE, 'a' + 1), "write in kernel");
		memcheck(ptr, 'a' + 1, size);

		SCIE(ioctl(d, IOCTL_DEMO_UNMAP, NULL), "unmap from user");
		//printproc("demo");
		//klog_show_clear();
		waitkey();
	}
	if (do_stress) {
		do_prog_init();
		for (unsigned int i = 0; i < count; i++) {
			do_prog(i, 10, count);
			unsigned int size = (i % 1000 + 1) * 4096;
			char c = i % 20 + 'a';
			scpe(ptr = (void *)ioctl(d, IOCTL_DEMO_MAP, size), "map to user");
			// set the buffer to c
			memset(ptr, c, size);
			// get buffer from user space
			scie(ioctl(d, IOCTL_DEMO_READ, c), "read in kernel");
			// ask the kernel to write c+1
			scie(ioctl(d, IOCTL_DEMO_WRITE, c + 1), "write in kernel");
			// check that the buffer has the right data
			memcheck(ptr, c + 1, size);
			// get ridd of the in kernel buffer
			scie(ioctl(d, IOCTL_DEMO_UNMAP, NULL), "unmap from user");
		}
		do_prog_finish();
	}

	SCIE(close(d), "close");
	//printproc("demo");
	//klog_show_clear();
	waitkey();

	return(0);
}
