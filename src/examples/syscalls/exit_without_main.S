/*
 * this is an example of doing the exit system call in assembly...
 * you can see this is running ok if you use 'echo $?' after the run
 * and see the return code coming up ok...
 *
 * Notes:
 * - -nostdlib is needed to avoid duplicate symbol for _start and avoid
 * excess size of the executable.
 * - -Xlinker --strip-all is to reduce size of the executable
 * - -Xlinker --build-id=none is to remove the linkers build-id stamp and
 * so reduce the size of the executable even more.
 * - use 'ls -l' to see just how small the executable file is.
 * - use 'ldd' to see that this executable does not link with anything. 
 * - use 'objdump -D [exefile]' to see how little content this executable has.
 *
 * EXTRA_LINK_FLAGS=-nostdlib -Xlinker --strip-all -Xlinker --build-id=none
 */

#include <asm/unistd.h>
#include <syscall.h>

.text
.global _start
.type _start, @function
_start:
	## the system call we want (from syscall.h)
	movl $SYS_exit, %eax
	## the value we want to return
	movl $7, %ebx
	## call the system call itself
	int $0x80
