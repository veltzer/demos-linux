include ../../../Makefile.mk
c_run.$(SUFFIX_BIN): c_run.cc
	$(info doing [$@])
	$(Q)gcc -I../../include -Wall -Werror -O3 -s -o $@ $<
.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f c_run.$(SUFFIX_BIN) Makefile.stamp
