hello: hello.o libadd.so 
	gcc -o hello hello.o -L. -ladd 
hello.o: hello.c
	gcc -g3 -c hello.c -o hello.o
libadd.so: add.o
	gcc -shared -o libadd.so add.o
add.o: add.c
	gcc -g3 -c add.c -o add.o

.PHONY: clean
clean:
	rm -f *.o hello libadd.so
