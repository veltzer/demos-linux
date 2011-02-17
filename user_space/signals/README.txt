The important stuff:
- sig handling is ASYNCHRONEOUS. This means that the data structures that you handle in your main code should not be assumed to be correct inside your signal handler (you could have been in the middle of a linked list manipulation method...).
- how can you avoid this problem? block signals inside your main code in areas which handle your data structures. Is this a good idea? that depends on the signal. Some signals cannot be blocked. Other signals are stupid to block (SIGSEGV?). Usually it's not a good idea to block unless you are using signals as IPC or really think that the signal arriving is not an error condition.
- The same goes for you signal handler: It could be interrupted by yet another signal and so if it does data structure manipulation this could mean trouble. How do avoid this? Block signals from within the signal handler or set the signal up with sigaction(2) which lets you block from the get go. This is actually the reason why a signal is block by default inside it's own signal hanlder. This is also the reason you have to tell the kernel to unblock the signal if you do not explicitly return from the signal handler (otherwise the signal will remain blocked).
- Is block signals inside your signal hanlder good? Not really (see the replies above).
- So what is signal handling good for? It is usually a bad IPC mechanism because it is asynchroneous but it may be useful for RT people for thhat reason precisely (but requires more careful coding). It is usually a very urgent signal passing mechanism from app to app ("need to shutdown now!!!") or a way for the kernel to tell you that something which you have no way of preparing for in your code just happend

you either messed up bigtime (SIGFPE, SIGSEGV, SIGILL)

allowing you to handle timers without resorting to multi threading (SIGALRM)

allowing you to respons to events that you do not control (SIGTERM, SIGCHLD, SIGCONT)
