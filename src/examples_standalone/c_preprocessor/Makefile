CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: debug.$(SUFFIX_BIN) release.$(SUFFIX_BIN)

debug.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) $< -DTYPE=DEBUG -o $@
release.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) $< -DTYPE=RELEASE -o $@

.PHONY: clean
clean:
	$(Q)rm -f *.$(SUFFIX_BIN)
