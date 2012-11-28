# variables
#CC:=icc
CC:=icpc
#CFLAGS:=-I/usr/include/i386-linux-gnu
CFLAGS:=

# targets
.PHONY: all
all: main.exe

main.exe: main.cc
	$(Q)$(CC) $(CFLAGS) main.cc -o main.exe

.PHONY: clean
clean:
	$(Q)-rm -f main.exe main.dis

.PHONY: dis
dis:
	$(Q)objdump main.exe --disassemble > main.dis
