include ../../../Makefile.defs

# the -pg is to generate profile info...
main.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ -pg -I../../include -O2 -o $@ $<
.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN) *.stamp
