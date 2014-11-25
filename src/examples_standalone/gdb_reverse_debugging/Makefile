include ../../../Makefile.mk

# variables
CXXFLAGS:=-I../../include -g3 -Wall -Werror
#CXXFLAGS:=-I../../include -g3 -O2 -Wall -Werror

# rules
.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN)
