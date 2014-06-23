include ../../../Makefile.mk

CXXFLAGS:=-I../../include -O2 -Wall -Werror @options

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.cc
	$(Q)g++ $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f main.$(SUFFIX_BIN)
