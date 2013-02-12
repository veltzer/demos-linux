include ../../../Makefile.defs
c_run.$(SUFFIX_BIN): c_run.cc
	$(Q)gcc -I../../include -Wall -Werror -O3 -s -o $@ $<
.PHONY: clean
clean:
	$(Q)-rm -f c_run.$(SUFFIX_BIN) Makefile.stamp
