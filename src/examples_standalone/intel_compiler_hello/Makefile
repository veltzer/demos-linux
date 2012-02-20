# variables
CC:=icc
CFLAGS:=-I/usr/include/i386-linux-gnu

# targets
.PHONY: all
all: main.exe

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe

.PHONY: clean
clean:
	-rm -f main.exe main.dis

.PHONY: dis
dis:
	objdump main.exe --disassemble > main.dis
