include ../../../Makefile.mk

CFLAGS:=-I../../include

.PHONY:all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -g3 -o $@ $<

.PHONY: debug
debug: main.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)gdb main.$(SUFFIX_BIN) -batch -x script.gdb

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.$(SUFFIX_BIN) core *.stamp
