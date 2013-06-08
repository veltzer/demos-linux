# An example of how to write a classic "Hello, World!\n" program in assembly

#include <asm/unistd.h>
#include <syscall.h>

.section	.rodata
hello:
	.string	"Hello, World!\n"
.text
.global	main
main:
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
