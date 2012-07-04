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
#include<sys/types.h> // for getpid(2), ftruncate(2)
#include<unistd.h> // for getpid(2), sleep(3), nice(2), fork(2), ftruncate(2)
#include<sched.h> // for sched_setaffinity(2), CPU_ZERO(3), CPU_SET(3)
#include<stdio.h> // for printf(3), fflush(3)
#include<us_helper.h> // for CHECK_ZERO(), CHECK_NOT_M1(), TRACE()
#include<sys/mman.h> // for shm_open(3), shm_unlink(3)
#include<sys/stat.h> // for shm_open(3), shm_unlink(3)
#include<fcntl.h> // for shm_open(3), shm_unlink(3)

/*
* This examples shows the effect of the nice scheduling system.
*
* It binds the process to a single core (for multi core systems).
* It then forks two versions of heavy work each with a different nice level.
* Then you can compare the work done by each of these.
*
* EXTRA_LIBS=-lrt
*
* References:
* man 2 nice
*/

void make_child(const int niceval,int* prog) {
	pid_t pid;
	CHECK_NOT_M1(pid=fork());
	if(pid!=0) { // parent
		return;
	}
	CHECK_NOT_M1(nice(niceval));
	float sum;
	for(unsigned int i=0;i<100000000;i++) {
		for(unsigned int j=0;j<10000000;j++) {
			sum+=j;
		}
		*prog=*prog+1;
	}
}

int main(int argc,char** argv,char** envp) {
	const int niceval=5;
	const char* shm_name="shared_mem";
	// bind the entire process to a single CPU (cpu 0)
	// lets get our pid (no error for getpid)
	pid_t mypid=getpid();
	cpu_set_t onecore;
	CPU_ZERO(&onecore);
	CPU_SET(0,&onecore);
	CHECK_NOT_M1(sched_setaffinity(mypid,sizeof(onecore),&onecore));
	// put this in shared memory
	CHECK_NOT_M1(shm_unlink(shm_name));
	int smfd;
	CHECK_NOT_M1(smfd=shm_open(shm_name,O_CREAT|O_RDWR,0));
	// we have to set the size otherwise it will not work
	CHECK_NOT_M1(ftruncate(smfd,getpagesize()));
	void* ptr;
	CHECK_NOT_VOIDP(ptr=mmap(
		NULL,/* addr: dont recommend address */
		getpagesize(),/* size: the size of the file */
		PROT_READ|PROT_WRITE,/* prot: we just want read */
		MAP_SHARED, /* flags: PRIVATE or SHARED ** MUST** be specified */
		smfd,/* fd: our file descriptor */
		0/* offset: from the begining of the file */
	),MAP_FAILED);
	int* iptr=(int*)ptr;
	*(iptr+0)=0;
	*(iptr+1)=0;
	// lets fork...
	make_child(niceval+1,iptr+0);
	make_child(niceval,iptr+1);
	int loop=0;
	while(true) {
		__sync_synchronize();
		printf("%d: prog1/2 is %d/%d\r",loop,*(iptr+0),*(iptr+1));
		fflush(stdout);
		sleep(1);
		loop++;
	}
	CHECK_NOT_M1(shm_unlink(shm_name));
	return EXIT_SUCCESS;
}
