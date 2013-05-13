/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for malloc(3), EXIT_SUCCESS
#include <unistd.h> // for pause(2), getpid(2)
#include <sys/types.h> // for getpid(2)
#include <sys/mman.h> // for mprotect(2)

/*
 * This example shows that regular strings (char*) are not protected
 * while const char* strings are protected (put in read only
 * memory). It also shows the use of mprotect(2) to change protection
 * on regions of memory.
 */

char mystring[100]="asdkjfhasdfkjhasdfsadfsadfasd";
const char* myotherstring="Hello, World!";

int main(int argc,char** argv,char** envp) {
	fprintf(stderr,"mystring is %p\n",mystring);
	fprintf(stderr,"myotherstring is %p\n",myotherstring);
	fprintf(stderr,"mypid is %d\n",getpid());
	//while(pause()) {
	//}
	int modulu=(int)myotherstring%4096;
	void* protectme=(void*)(myotherstring-modulu);
	mprotect(protectme,4096,PROT_READ|PROT_WRITE|PROT_EXEC);
	char* p=(char*)myotherstring;
	*(p+4)='e';
	fprintf(stderr,"myotherstring is %s\n",myotherstring);
	/*
	char* p=(char*)malloc(10);
	fprintf(stderr,"after malloc with p=%p\n",p);
	*/
	/*
	for(int i=0;i<1000000;i++) {
		*p=(char)random();
		if(i%1000==0) {
			fprintf(stderr,"Yes!, worked for i=%d\n",i);
		}
		p++;
	}
	*/
	return EXIT_SUCCESS;
}
