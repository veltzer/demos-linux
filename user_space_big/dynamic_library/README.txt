This demo shows all the required steps needed in order to make a dynamic library:
- write a bunch of c/c++ files with headers interface files.
- compile them all, prefereably with -O2 and -Wall -WerrorO.
- link them with -shared -fpic (-fpic not strictly needed on Intel).
- library name should be lib[libname].so
- compile an application.
- link the application with -L[path to library] -l[libname].
- -L in previous link is not needed if the library is installed in standard paths.
- run the application with LD_LIBRARY_PATH pointing to where the library is.
- LD_LIBRARY_PATH in the previous statement is not needed if the library is installed in
	standard paths.
- have a look at my Makefile to see how to write a makefile for such a library.
