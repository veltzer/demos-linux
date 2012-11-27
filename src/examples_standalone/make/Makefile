CFLAGS:=-Wall -Werror -I../../include

prog.exe: main.o add.o
	gcc -o $@ main.o add.o 
main.o: main.c
	gcc $(CFLAGS) -c -o $@ $<
add.o: add.c
	gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm -f *.o prog.exe
