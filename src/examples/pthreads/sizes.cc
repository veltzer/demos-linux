/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep(3)
#include <strings.h> // for bzero(3)
#include <us_helper.h> // for CHECK_ZERO()

/*
* This is an example of setting thread stack sizes
* If you want to see the minor page faults happening while this
* example is running run:
* while [[ true ]]; do ps -C sizes.exe -o comm,min_flt,rss; sleep 1;done
*
* EXTRA_LINK_FLAGS=-lpthread
*
* TODO:
* - show number of page faults for this application programmatically.
* First run a thread that allocates it's space in advance.
* Then run a thread that does not.
* show the info from /proc
*/

void ensure_space(unsigned int size) {
	// two ways of making sure there is enough space on the stack...
	int dummy;
	bzero(&dummy-size/2,size/2);
	//char buf[size];
	//bzero(buf,size);
}

void *worker(void *p) {
	int num=*(int *)p;
	ensure_space((num+1)*100*1024);
	fprintf(stderr, "starting thread %d\n", num);
	sleep(100);
	fprintf(stderr, "ending thread %d\n", num);
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	const int num=10;
	pthread_t threads[num];
	pthread_attr_t attrs[num];
	int ids[num];
	void* rets[num];

	fprintf(stderr, "main starting\n");
	for(int i=0;i<num;i++) {
		ids[i]=i;
		CHECK_ZERO(pthread_attr_init(attrs+i));
		CHECK_ZERO(pthread_attr_setstacksize(attrs+i,(i+1)*1024*1024));
		CHECK_ZERO(pthread_create(threads + i, attrs+i, worker, ids + i));
		CHECK_ZERO(pthread_attr_destroy(attrs+i));
		sleep(5);
	}
	fprintf(stderr, "main ended creating threads\n");
	for(int i=0;i<num;i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	fprintf(stderr, "main ended\n");
	return EXIT_SUCCESS;
}
