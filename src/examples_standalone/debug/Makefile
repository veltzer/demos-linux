CFLAGS:=-I../../include

.PHONY:all
all: main.forclient.$(SUFFIX_BIN) main.forhq.$(SUFFIX_BIN)

main.forclient.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) -s -g3 -o $@ $<
main.forhq.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) -g3 -o $@ $<
clean:
	$(Q)-rm -f main.forclient.$(SUFFIX_BIN) main.forhq.$(SUFFIX_BIN) core
