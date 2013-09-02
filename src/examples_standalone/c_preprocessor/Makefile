include ../../../Makefile.defs

CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: debug.$(SUFFIX_BIN) release.$(SUFFIX_BIN)

debug.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) $< -DTYPE=DEBUG -o $@
release.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) $< -DTYPE=RELEASE -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)rm -f *.$(SUFFIX_BIN) *.stamp
