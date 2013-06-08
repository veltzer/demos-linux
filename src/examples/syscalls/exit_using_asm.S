/*
 * this is an example of doing the exit system call in assembly...
 * you can see this is running ok if you use 'echo $?' after the run
 * and see the return code coming up ok...
 */

#include <asm/unistd.h>
#include <syscall.h>

.text
.global main
.type main, @function
main:
	## the system call we want (from syscall.h)
	movl $SYS_exit, %eax
	## the value we want to return
	movl $7, %ebx
	## call the system call itself
	int $0x80
