include ../../../Makefile.mk

ARCH:=$(shell arch)
ifneq ($(ARCH), i686)
ALL:=hello.elf
endif
#ARCH:=elf32
ARCH:=elf64

.PHONY: all
all: $(ALL)
	$(info doing [$@])

hello.elf: hello.o
	$(info doing [$@])
	$(Q)ld -o $@ $<

hello.o: hello.asm
	$(info doing [$@])
	$(Q)nasm -f $(ARCH) -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) hello.o
