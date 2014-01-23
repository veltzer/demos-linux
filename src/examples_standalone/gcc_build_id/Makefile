include ../../../Makefile.defs

CXXFLAGS:=-I../../include -O2 -Wall -Werror

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.cc
	$(Q)g++ $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f main.$(SUFFIX_BIN)

.PHONY: show
show: all
	file main.$(SUFFIX_BIN)
	nm -a main.$(SUFFIX_BIN) | grep build-id
	readelf -x .note.gnu.build-id main.$(SUFFIX_BIN)
