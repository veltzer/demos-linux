include ../../../Makefile.mk

ALL:=hello.elf

.PHONY: all
all: $(ALL)
	$(info doing [$@])

hello.elf: hello.o
	$(info doing [$@])
	$(Q)ld -o $@ $<

hello.o: hello.asm
	$(info doing [$@])
	$(Q)nasm -f elf64 -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) hello.o
