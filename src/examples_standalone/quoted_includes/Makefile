.PHONY: all
all:
	gcc -c main.c -Ifolder2 -Ifolder1
	gcc -c main.c -I- -Ifolder2 -Ifolder1
	icpc -c main.c -Ifolder2 -Ifolder1
	icpc -c main.c -I- -Ifolder2 -Ifolder1

.PHONY: clean
clean:
	rm -f main.o
