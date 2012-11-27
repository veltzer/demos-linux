CFLAGS:=-I../../include

.PHONY: all
all: main.exe libadd.so

main.exe: main.o 
	gcc -o $@ $< -ldl
main.o: main.c
	gcc $(CFLAGS) -c -o $@ $<
libadd.so: add.o
	gcc -shared -o $@ $<
add.o: add.c
	gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm *.o main.exe libadd.so
