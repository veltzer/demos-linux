# variables
CC:=icc
# this is no optimization
#CFLAGS:=-O0 -I/usr/include/i386-linux-gnu -I../../include
# in the Intel compiler -O, -O2 or no flag for optimization are the same
# none of following flags makes the intel compiler actually show a difference between
# alias and non-alias...
CFLAGS:=-no-simd -fargument-alias -no-opt-multi-version-aggressive -I/usr/include/i386-linux-gnu -I../../include

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
