program: main.o other.o
	gcc main.o other.o -o program

main.o: main.c
	gcc -c -o main.o main.c
other.o: other.c
	gcc -c -o other.o other.c

clean:
	-rm -f *.o program
