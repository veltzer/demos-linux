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

#include <firstinclude.h>
#include <sys/inotify.h> // for inotify_init(2), inotify_add_watch(2)
#include <stdio.h> // for printf(3)
#include <unistd.h> // for read(2), close(2)
#include <assert.h> // for assert(3)
#include <errno.h> // for errno(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)
#include <signal.h> // for siginterrupt(2)
#include <limits.h> // for PATH_MAX
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This demo shows how to use the inotify(2) API to get notifications of file changes.
*
* Getting out of the inotify main loop:
* We use a signal to break out of the inotify loop. We could have used poll or an event
* fd from other threads which would work out just fine. We use siginterrupt and a user
* defined signal instead. That is the reason for all the singla handling code in this
* example.
*
* One of the weird things in terms of the inotify API is that it returns records of uneven
* length. Each record is of size sizeof(inotity_event)+ie->len. The idea is to save on short
* file name lengths. And a single read can return more than one record but will always return
* an even amount of records.
*/

// signal handling functions
static bool stop=false;
static void handler(int sig, siginfo_t *si, void *unused) {
	printf("got signal, you probably want me to stop so I'm stopping...\n");
	stop=true;
}
static void register_handler() {
	struct sigaction sa;
	sa.sa_flags=SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction=handler;
	CHECK_NOT_M1(sigaction(SIGUSR1, &sa, NULL));
}
// print an inotify mask in readable form
static uint32_t types[]={
	IN_ACCESS, IN_ATTRIB, IN_CLOSE_WRITE,
	IN_CLOSE_NOWRITE, IN_CREATE, IN_DELETE,
	IN_DELETE_SELF, IN_MODIFY, IN_MOVE_SELF,
	IN_MOVED_FROM, IN_MOVED_TO, IN_OPEN };
static const char* names[]={
	"IN_ACCESS", "IN_ATTRIB", "IN_CLOSE_WRITE",
	"IN_CLOSE_NOWRITE", "IN_CREATE", "IN_DELETE",
	"IN_DELETE_SELF", "IN_MODIFY", "IN_MOVE_SELF",
	"IN_MOVED_FROM", "IN_MOVED_TO", "IN_OPEN" };
static void print_mask(uint32_t mask) {
	bool first=true;
	for(unsigned int i=0;i<ARRAY_SIZEOF(types);i++) {
		uint32_t m=types[i];
		if(mask & m) {
			if(first)
				printf("%s",names[i]);
			else
				printf("-%s",names[i]);
		}
	}
}
// to count the maximum number of records that inotify read returns...
static int max_rec=0;
static int max_len=0;

int main(int argc,char** argv,char** envp) {
	register_handler();
	CHECK_NOT_M1(siginterrupt(SIGUSR1,1));
	int fd;
	CHECK_NOT_M1(fd=inotify_init());
	uint32_t mask=IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM;
	const char* path="/tmp";
	CHECK_NOT_M1(inotify_add_watch(fd,path,mask));
	printf("fd is %d\n", fd);
	printf("stop me with [kill -s SIGUSR1 %d]\n",getpid());
	printf("trace me with [strace -p %d]\n",getpid());
	printf("watching [%s] - change files to see me respond...\n",path);
	while(!stop) {
		const int size=sizeof(inotify_event)+PATH_MAX+1;
		// large buffer for testing purposes...
		//const int size=100000;
		char* buf[size];
		int res=read(fd,buf,size);
		if(res==-1) {
			int err=errno;
			if(err!=EINTR) {
				CHECK_NOT_M1(res);
			} else {
				continue;
			}
		}
		int i=0;
		int rec_num=0;
		while(i<res) {
			struct inotify_event* ie=(struct inotify_event*)(buf+i);
			// print the event
			printf("event-> file %s, mask ",ie->name);
			print_mask(ie->mask);
			printf("\n");
			int rec_len=sizeof(inotify_event)+ie->len;
			if(rec_len>max_len)
				max_len=rec_len;
			i+=rec_len;
			rec_num++;
		}
		if(rec_num>max_rec)
			max_rec=rec_num;
		// each read should return an exact number of records
		assert(i==res);
	}
	CHECK_NOT_M1(close(fd));
	printf("max_rec is %d\n",max_rec);
	printf("max_len is %d\n",max_len);
	return EXIT_SUCCESS;
}
