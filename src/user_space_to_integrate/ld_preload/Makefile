all: main libmylib.so

main: main.cc
	g++ -o main main.cc

libmylib.so: lib.cc
	g++ -fpic -shared -o libmylib.so lib.cc

clean:
	-rm -f main libmylib.so
