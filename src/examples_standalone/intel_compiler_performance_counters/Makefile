include ../../../Makefile.defs

GCC_MACHINE:=$(shell gcc -dumpmachine)
ifeq ($(GCC_MACHINE),x86_64-linux-gnu)
	ARCH:=x86_64
else
	ARCH:=i386
endif
FOLDER:=/usr/include/$(ARCH)-linux-gnu/c++/4.8
CFLAGS:= -I$(FOLDER) -I../../include -Wall -Werror
CC:=icpc

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
