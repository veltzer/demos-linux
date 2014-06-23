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
clean:
	$(info doing [$@])
	$(Q)-rm -f main.forclient.$(SUFFIX_BIN) main.forhq.$(SUFFIX_BIN) core *.stamp
