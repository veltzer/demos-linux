These are various demo drives that I play with to prove or to build
prototypes for the stuff I put into the nusupport package...

There is also a list of user space test programs to test these (test_*.c).

To build try something out:
1. Symlink demo.c (the driver code) to the driver you want to test (drv_XXX.c).
2. "make clean"
3. "make"
4. now run the relevant test program (./test_XXX).
