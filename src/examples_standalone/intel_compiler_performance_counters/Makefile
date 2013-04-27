# variables
SUFFIX_BIN?=elf
CC:=icpc
CFLAGS:=-I/usr/include/i386-linux-gnu/c++/4.7 -I../../include -Wall -Werror

# targets
.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.cc
	$(Q)$(CC) $(CFLAGS) main.cc -o main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(Q)-rm -f main.$(SUFFIX_BIN) main.dis

.PHONY: dis
dis:
	$(Q)objdump main.$(SUFFIX_BIN) --disassemble > main.dis
