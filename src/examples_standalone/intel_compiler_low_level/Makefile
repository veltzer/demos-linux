# variables
CC:=icc
CFLAGS:=-Wall -Werror -I../../include

# targets
.PHONY: all
all: main.exe

main.exe: main.c
	$(Q)$(CC) $(CFLAGS) main.c -o main.exe

.PHONY: clean
clean:
	$(Q)-rm -f main.exe

.PHONY: dis
dis:
	$(Q)objdump main.exe --disassemble > main.dis
