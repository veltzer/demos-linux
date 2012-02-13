# These are the libraries used by your library
LIBS=
# on Intel this usually works without the -fpic but use it none
# the less, you can also add -nostdlib to avoid the default 
# bahviour of linking with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB=-shared -fpic
all: main libmylib.so
CXX=g++
libmylib.so: mylib.cc
	$(CXX) $(FLAGS_FOR_SHARED_LIB) -o $@ $< $(LIBS)
main: main.cc libmylib.so
	$(CXX) -o $@ $< -L. -lmylib
clean:
	-rm libmylib.so main

show_results:
	ldd ./libmylib.so
	ldd ./main
	-./main

show_results_verbose:
	ldd -v ./libmylib.so
	ldd -u ./libmylib.so

run:
	LD_LIBRARY_PATH=$(PWD) ./main 5.6
