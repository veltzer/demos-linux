/*
 * This is an extension of the exit_without_main.S example. See
 * its documentation for more info.
 *
 * EXTRA_LINK_FLAGS=-nostdlib -Xlinker --strip-all -Xlinker --build-id=none
 */

#include <asm/unistd.h>
#include <syscall.h>

.section	.rodata
hello:
	.string	"Hello, World!\n"
.text
.global _start
.type _start, @function
_start:
	## write(1,hello,14)
	movl $SYS_write, %eax
	movl $1, %ebx
	movl $hello, %ecx
	movl $14, %edx
	int $0x80
	
	## _exit(7)
	movl $7, %ebx
	movl $SYS_exit, %eax
	int $0x80
