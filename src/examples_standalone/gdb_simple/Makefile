include ../../../Makefile.mk

CFLAGS:=-I../../include

.PHONY:all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -g3 -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.$(SUFFIX_BIN) core *.stamp

.PHONY: debug
debug:
	$(info doing [$@])
	gdb main.$(SUFFIX_BIN) -ex run -ex list -batch
