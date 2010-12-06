This project demonstrates a single Makefile to compile an entire
C/C++ project.

Things to notice:
1. The file is called Makefile since this file is searched by default
by gnu make.
2. The CC variable is not replaced. What for ? gcc is already the default compiler on GNU/Linux systems.
3. The deps files are included using the "-include" and not the "include" statement. This is to avoid warnings when some or all of the deps files are not yet available.
4. The rule for creating deps files is IMPLICIT. Unlike all other rules in the Makefile.
5. The rule for compiling code is not even in the Makefile. It is based on the implicit default rule built into gmake.
6. The ld_source_me file is needed to set up the LD_LIBRARY_PATH dynamic linker in order for it to find the libadd.so lib which is in this directory.

TODO:
- deps still get generated even if we do "make clean". Add the include under a condition that the target is not "clean".
