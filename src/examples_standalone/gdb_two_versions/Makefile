include ../../../Makefile.mk

CFLAGS:=-I../../include

.PHONY:all
all: main.forclient.$(SUFFIX_BIN) main.forhq.$(SUFFIX_BIN)

main.forclient.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -s -g3 -o $@ $<
main.forhq.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -g3 -o $@ $<

.PHONY: debug
debug:
	$(info doing [$@])
	$(Q)ulimit -c unlimited; ./main.forclient.$(SUFFIX_BIN); exit 0
	$(Q)gdb --core=core ./main.forhq.$(SUFFIX_BIN) -batch -ex list

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.$(SUFFIX_BIN) core *.stamp
