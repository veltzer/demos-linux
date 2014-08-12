include ../../../Makefile.mk

CXXFLAGS:=-I. -I../../include -O2 -Wall -Werror

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): one.cc two.cc main.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN)
