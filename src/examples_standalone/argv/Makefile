include ../../../Makefile.defs

.PHONY: all
all: hello.out goodbye.out

hello_or_goodbye.$(SUFFIX_BIN): hello_or_goodbye.c
	$(Q)gcc -I../../include -Wall -Werror -O3 -s -o $@ $<

hello.out: hello_or_goodbye.$(SUFFIX_BIN)
	ln -s $< $@
goodbye.out: hello_or_goodbye.$(SUFFIX_BIN)
	ln -s $< $@

.PHONY: clean
clean:
	$(Q)-rm -f hello_or_goodbye.$(SUFFIX_BIN) hello.out goodbye.out Makefile.stamp
