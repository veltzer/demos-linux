# variables
CC:=icc
CFLAGS:=-Wall

# targets
.PHONY: all
all: main.exe

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe

.PHONY: clean
clean:
	-rm -f main.exe

.PHONY: dis
dis:
	objdump main.exe --disassemble > main.dis
