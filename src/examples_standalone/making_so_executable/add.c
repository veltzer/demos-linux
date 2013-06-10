/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h> // for printf(3)
#include <unistd.h> // for write(2), _exit(2)
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/syscall.h> // for syscall(2)
#include <string.h> // for strlen(3)
#include <stdbool.h>	// for true

/*
 * Must implement the add function, as main actually calls it...
 */
int add(int a,int b) {
	return a+b;
}
/*
void do_init(void) __attribute__((constructor));
void do_init(void) {
	printf("Hello\n");
}
*/

// this succeeds
inline void do_exit() {
	// SYS_exit is 1 (from /usr/include/i386-linux-gnu/asm/unistd_32.h)
	// argument is in ebx, it is 7 so that you can see it from the
	// command line
	// You can zero ebx using the following command (that is what the
	// compiler usually does...):
	// xor %ebx,%ebx
	// entering kernel mode is done via the 'int $0x80'
	// the semincolon after each statement is mandatory since it is what
	// enables the assembler to distinguish between different machine
	// instructions...
	__asm__ volatile(		\
		"mov $1,%eax;"		\
		"mov $7,%ebx;"		\
		"int $0x80;"		\
	);
}

// this fails
inline void do_write() {
	// lets try a write syscall to file descriptor 1...
	// strcpy(mybuf.buf,buf_data);
	// const int len=strlen(buf_data);
	// const char* buf=(const char*)0x700;
	// const char* buf=(const char*)(buf_data);
	// const char* mybuf=buf_data;
	// SYS_write is 4 (from /usr/include/i386-linux-gnu/asm/unistd_32.h) (eax)
	// file descriptor is 1 (ebx)
	// buffer (ecx)
	// size of buffer (ecx)
	// enter kernel mode

	// next block of code does not compile - fix it...
	/*
	long res;
	const char buf_data[]="Hello world from the lib\n";
	const int len=strlen(buf_data);
	__asm__ volatile(		\
		"mov $4,%eax;"		\
		"mov $1,%ebx;"		\
		"mov %0,%ecx;"		\
		"mov %1,%edx;"		\
		"int $0x80;"		\
		: "=a" (res)		\
		:"r" ((long)(buf_data)), "r" ((long)(len))	\
	);
	*/
}

// this succeeds
inline void do_endless_loop() {
	volatile int i=0;
	while(true) {
		i++;
	}
}

// all of these fail
inline void do_standard_stuff() {
	printf("hello from the lib...\n");
	syscall(SYS_exit);
	_exit(EXIT_FAILURE);
	int ret=write(1,"hello\n",6);
	if(ret!=6) {
		_exit(EXIT_FAILURE);
	}
}

//int main(int argc,char** argv,char** envp) __attribute__((weak,noreturn));
//int main(int argc,char** argv,char** envp) __attribute__((weak));
//int mymain(int argc,char** argv,char** envp) __attribute__((noreturn));
int mymain(int argc,char** argv,char** envp) {
	/*
	__asm__ volatile(						\
		"int $0x80"						\
		: "=a" (res)						\
		: "0" (4),"b" ((long)(1)),"c" ((long)(other_buf)),	\
		"d" ((long)(5))						\
	);
	*/

	/*
	asm(
		// SYS_exit is 1 (from /usr/include/i386-linux-gnu/asm/unistd_32.h)
		"mov $1,%eax;"
		// argument is in ebx, it is 0
		"xor %ebx,%ebx;"
		// enter kernel mode
		"int $0x80;"
	);
	*/
	// even this fails
	return EXIT_SUCCESS;
}
