# variables
CC:=icc
CFLAGS:=-I/usr/include/i386-linux-gnu

# targets
.PHONY: all
all: main.exe main.dis

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe

main.dis: main.exe
	objdump main.exe --disassemble > main.dis

.PHONY: clean
clean:
	-rm -f main.exe main.dis

.PHONY: dis
dis: main.dis
