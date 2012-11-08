# variables
#CC:=icc
CC:=icpc
CFLAGS:=-gcc-version=460 -I/usr/include/i386-linux-gnu

# targets
.PHONY: all
all: main.exe

main.exe: main.cc
	$(CC) $(CFLAGS) main.cc -o main.exe

.PHONY: clean
clean:
	-rm -f main.exe main.dis

.PHONY: dis
dis:
	objdump main.exe --disassemble > main.dis
