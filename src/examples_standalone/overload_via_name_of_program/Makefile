include ../../../Makefile.mk

.PHONY: all
all: hello.out goodbye.out

hello_or_goodbye.$(SUFFIX_BIN): hello_or_goodbye.c
	$(info doing [$@])
	$(Q)gcc -I../../include -Wall -Werror -O3 -s -o $@ $<

hello.out: hello_or_goodbye.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)rm -f $@
	$(Q)ln -s $< $@
goodbye.out: hello_or_goodbye.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)rm -f $@
	$(Q)ln -s $< $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f hello_or_goodbye.$(SUFFIX_BIN) hello.out goodbye.out Makefile.stamp
