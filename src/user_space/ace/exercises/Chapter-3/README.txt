Exercise 3
Write a program that will update a single variable called LockedCounter.
We have to protect the variable so that only one thread will be able to update it.
We will use 3 threads.
1. The initial value of LockedCounter is zero.
2. Use 3 threads each one of them will try to increment the LockedCounter.
3. Each thread will use mutex to prevent other thread from updating or testing
   this variable at the same time.
4. Only if LockedCounter modulu 3 is = n then thread n will do the increment.
5. Each thread should make X attemts (constant) to change the LockedCounter.
6. At the end print the value of LockedCounter as well as the total number of
   attempts for changing the counter.
7. What do you deduce from the results of this exercise? (the difference in values between
	the LockedCounter and the attempts...). Could you suggest a way to improve on the
	situation?
