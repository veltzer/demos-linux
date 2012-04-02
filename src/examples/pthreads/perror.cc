/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<firstinclude.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include<us_helper.h>

/*
* This is a demo which shows that perror, errno etc all work on a thread
* by thread level...
*
* EXTRA_LIBS=-lpthread
*/
void *worker1(void *p) {
	fprintf(stderr, "worker1 starting thread\n");
	fprintf(stderr, "test 1: with no error\n");
	int err = errno;
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "test 2: doing something illegal\n");
	open("/etc/nonexistant", O_RDWR);
	err = errno;
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "test 3: setting error myself\n");
	errno = 3;
	err = 3;
	perror("msg from perror");
	fprintf(stderr, "errno is %d (%s)\n", err, strerror(err));
	fprintf(stderr, "worker1 ending thread\n");
	return(NULL);
}


void *worker2(void *p) {
	fprintf(stderr, "worker2 starting thread\n");
	fprintf(stderr, "errno is %d\n", errno);
	perror("printing perror");
	fprintf(stderr, "worker2 ending thread\n");
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	pthread_t thread1;
	pthread_t thread2;
	const int num = 2;
	int ids[num];
	void* rets[num];

	fprintf(stderr, "main starting\n");
	CHECK_ZERO(pthread_create(&thread1, NULL, worker1, ids + 0));
	CHECK_ZERO(pthread_create(&thread2, NULL, worker2, ids + 1));
	fprintf(stderr, "main ended creating threads\n");
	CHECK_ZERO(pthread_join(thread1, rets + 0));
	CHECK_ZERO(pthread_join(thread1, rets + 1));
	fprintf(stderr, "main ended\n");
	return EXIT_SUCCESS;
}
