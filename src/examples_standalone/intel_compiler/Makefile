# variables
CC:=~/install/intel_32bit/bin/icc
CFLAGS:=-I/usr/include/i386-linux-gnu

# targets
.PHONY: all
all: main.exe

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe

.PHONY: clean
clean:
	-rm -f main.exe
