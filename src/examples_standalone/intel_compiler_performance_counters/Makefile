include ../../../Makefile.mk

GCC_MACHINE:=$(shell gcc -dumpmachine)
ifeq ($(GCC_MACHINE),x86_64-linux-gnu)
	ARCH:=x86_64
else
	ARCH:=i386
endif
FOLDER:=/usr/include/$(ARCH)-linux-gnu/c++/4.8
CFLAGS:= -I$(FOLDER) -I../../include -Wall -Werror
CC:=icpc

ifeq ($(DO_INTEL),1)
	ALL:=main.$(SUFFIX_BIN)
else
	ALL:=
endif

# targets
.PHONY: all
all: $(ALL)

main.$(SUFFIX_BIN): main.cc
	$(Q)$(CC) $(CFLAGS) main.cc -o main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(Q)-rm -f main.$(SUFFIX_BIN) main.dis

.PHONY: dis
dis:
	$(Q)objdump main.$(SUFFIX_BIN) --disassemble > main.dis
