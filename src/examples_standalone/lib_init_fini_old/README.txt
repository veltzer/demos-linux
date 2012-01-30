This demostration show that:
1. You can write you own init and finish function to be executed when an application starts
	using your dll and when it finishes so.
2. These hooks will only be called once per application and will NOT be called for threads
	and forks.
3. You must register them using the right flags to the linker. 
	-Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini
4. You can also set a build id.
	-Xlinker --build-id=0xaabbcc
5. Notice that the order of initialization is derived from the order of linkage.
	Change the order of linkage and you get different initialization order.
