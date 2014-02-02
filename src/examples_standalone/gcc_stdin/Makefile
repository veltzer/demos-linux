include ../../../Makefile.defs

CXXFLAGS:=-I../../include -O2 -Wall -Werror

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)cat my_include.hh $< | g++ $(CXXFLAGS) -o $@ -x c++ - 

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN)
