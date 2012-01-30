all: main libadd.so
main: main.o 
	gcc -o main main.o -ldl
main.o: main.c
	gcc -c -o main.o main.c
libadd.so: add.o
	gcc -shared -o libadd.so add.o
add.o: add.c
	gcc -c -o add.o add.c
clean:
	-rm *.o main libadd.so
