include ../../../Makefile.mk

CXXFLAGS:=-Wall -Werror -I../../include

out.$(SUFFIX_BIN): A.o use_a.o
	$(info doing [$@])
	$(Q)g++ -o $@ A.o use_a.o
A.o: A.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -g3 -o $@ $<
use_a.o: use_a.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -g3 -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f A.o use_a.o out.$(SUFFIX_BIN) *.stamp

.PHONY: dump
dump:
	$(info doing [$@])
	$(Q)objdump --syms -C use_a.o
