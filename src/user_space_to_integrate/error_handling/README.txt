This list of demos explains some of the deeper issues of handling errors from C/C++
code in systems programming on a linux system.

// Creating errors (this is the easy part)

You can trace the actual signal sent by the OS using strace.

CreateFPE: demostrates a very short program creating a floating point exception.

CreateSEGV: an easy demostration about creating a segmentation fault.

// Catching signals

CatchSigs: A C level signal catching software using the signal(2) interface.
	Notice the following:
	- The non busy loop for waiting for signals in the main thread
	- The fact that the signal is blocked inside the signal handler
		while other signals are not.
		This effect the protection you get on your data
		structures.
	- Only one signal is "cached" while you are blocking that signal (the kernel holds a mask of pending signals for you).
	- If you want more than one signal cached then you should use the sigqueue(2) instead of the kill(2) interface and then you can also pass parameters to that handler (limit is according to ulimit).
	- I provided a SigQueue executable to allow you to do that. Use it to see the signals queued and the value that you provide cought by the process. 

// C

LongJmp: demostrates the use of longjmp in a C program to handle errors.

LongJmpStackTrace: demostrates longjmp with easy to use C wrappers
	which are thread safe and name demangling.

// C++

ThrowCatchSigs: demostrates C++ style exception handling as response to OS signals.
	Things to notice:
	- the need to unblock the signal in the signal handler
	- the fact that raise(3) and kill(2) do not work exactly the same.

StackedTracedExceptions: demostrates how to create stack traced exceptions in C++.
	Notice that if you compile with -O2 you get less stack frames in the output
	since some of the functions are inlined by the compiler.
	If you want to see all stack frames then remove all optimization flags at
	compile time.
	Another note is the fact that we drop two stack frames that don't interest
	us (two functions that are above the 'main' function).
	Yet another issue is C++ name mangling. You can either do that on the command
	line using the 'c++filt' tool or in the code by using the C++ library.

ErrorHandle: example of throwing all OS interfaces as C++ exception and cutting on the size of code you need to write.

Syncronization and Asynchronization

BreakOut: demostrates how to cause a thread that is stuck in a long system call to break out of it. The idea is to generate a signal and to define that signal as an interrupt signal. This will not cause the system call to be restarted but rather the system call (in this examples case read(2)) will return with an error(-1) and the error code will be -EINTR.
