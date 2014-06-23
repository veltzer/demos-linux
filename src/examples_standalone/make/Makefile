include ../../../Makefile.mk

CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: prog.$(SUFFIX_BIN)

prog.$(SUFFIX_BIN): main.o add.o
	$(info doing [$@])
	$(Q)gcc -o $@ main.o add.o 
main.o: main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<
add.o: add.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.o prog.$(SUFFIX_BIN)
