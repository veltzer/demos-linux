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

//#include<firstinclude.h>
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include<iostream> // for std::cout, std::endl

int main(int argc,char** argv,char** envp) {
	PCM* m=PCM::getInstance();
	// program counters, and on a failure just exit
	if(m->program()!=PCM::Success)
		return EXIT_FAILURE;
	SystemCounterState before_sstate=getSystemCounterState();
	long sum=0;
	for(int i=0;i<1000000) {
		for(int j=0;j<10000000) {
			int mu1;
			if(i%2==0) {
				mul=-1;
			}
			sum+=mul*j*j;
		}
	}
	SystemCounterState after_sstate=getSystemCounterState();
	std::cout << "Instructions per clock:" << getIPC(before_sstate,after_sstate)
		<< "L3 cache hit ratio:" << getL3CacheHitRatio(before_sstate,after_sstate)
		<< "Bytes read:" << getBytesReadFromMC(before_sstate,after_sstate)
		<< std::endl;
	return EXIT_SUCCESS;
}
