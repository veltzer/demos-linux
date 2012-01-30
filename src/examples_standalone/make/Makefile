prog: main.o add.o
	gcc -o prog main.o add.o
main.o: main.c
	gcc -c -o main.o main.c
add.o: add.c
	gcc -c -o add.o add.c

.PHONY: clean
clean:
	-rm -f *.o prog
