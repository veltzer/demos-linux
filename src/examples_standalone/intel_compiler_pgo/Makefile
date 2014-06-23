include ../../../Makefile.mk

# variables
CXX:=icc
#CXXFLAGS:=-I/usr/include/i386-linux-gnu
CXXFLAGS:=-Wall -Werror -O2 -I../../include
PROF_GEN=-prof-gen
PROF_USE=-prof-use

# targets
.PHONY: all
all: main_stage2.$(SUFFIX_BIN)

main_stage1.o: main.cc
	$(Q)$(CXX) $(CXXFLAGS) $(PROF_GEN) -c -o $@ $<
main_stage1.$(SUFFIX_BIN): main_stage1.o
	$(Q)$(CXX) -o $@ $<
main_run.stamp: main_stage1.$(SUFFIX_BIN)
	$(Q)rm -f *.dpi *.dyn *.lock
	$(Q)./main_stage1.$(SUFFIX_BIN) 1> /dev/null 2> /dev/null
	$(Q)touch main_run.stamp
main_stage2.o: main.cc main_run.stamp
	$(Q)$(CXX) $(CXXFLAGS) $(PROF_USE) -c -o $@ $<
main_stage2.$(SUFFIX_BIN): main_stage2.o
	$(Q)$(CXX) -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.dyn *.lock *.dpi main_stage?.o main_stage?.$(SUFFIX_BIN) *.stamp
