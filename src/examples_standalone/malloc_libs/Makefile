include ../../../Makefile.mk

ALL:=memory_error.dmalloc.$(SUFFIX_BIN) memory_error.regular.$(SUFFIX_BIN) memory_error.efence.$(SUFFIX_BIN)
ALL_DEPS:=Makefile
CXXFLAGS:=-O2 -g3 -Wall -Werror -I../../include

.PHONY: all
all: $(ALL) 

memory_error.regular.$(SUFFIX_BIN): memory_error.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -o $@ $<
memory_error.efence.$(SUFFIX_BIN): memory_error.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -o $@ $< -lefence
memory_error.dmalloc.$(SUFFIX_BIN): memory_error.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -DDMALLOC -o $@ $< -ldmalloc
.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) logfile.out
.PHONY: valgrind_memcheck
valgrind_memcheck:
	$(info doing [$@])
	$(Q)valgrind --tool=memcheck ./memory_error.regular.$(SUFFIX_BIN) 0
