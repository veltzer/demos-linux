# variables
CC:=icc
CFLAGS:=-Wall -Werror -I../../include

# targets
.PHONY: all
all: main.exe main.dis

main.exe: main.c
	$(Q)$(CC) $(CFLAGS) main.c -o main.exe

main.dis: main.exe
	$(Q)objdump main.exe --disassemble > main.dis

.PHONY: clean
clean:
	$(Q)-rm -f main.exe main.dis

.PHONY: dis
dis: main.dis
