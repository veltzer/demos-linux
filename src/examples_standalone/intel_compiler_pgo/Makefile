# variables
CXX:=icc
#CXXFLAGS:=-I/usr/include/i386-linux-gnu
CXXFLAGS:=-Wall -Werror -O2 -I../../include
PROF_GEN=-prof-gen
PROF_USE=-prof-use

# targets
main.$(SUFFIX_BIN): main_st2.o
	$(Q)$(CXX) -o $@ $<
main_st2.o: main.cc main.gcda
	$(Q)$(CXX) $(CXXFLAGS) $(PROF_USE) -c -o main.o main.cc
	$(Q)mv main.o main_st2.o
main.gcda main_run.stamp: main_phase1.$(SUFFIX_BIN)
	$(Q)./main_phase1.$(SUFFIX_BIN) 1> /dev/null 2> /dev/null
	$(Q)touch main_run.stamp
main_phase1.$(SUFFIX_BIN): main.o
	$(Q)$(CXX) -o $@ $<
main.o: main.cc
	$(Q)$(CXX) $(CXXFLAGS) $(PROF_GEN) -c -o main.o main.cc
.PHONY: clean
clean:
	$(Q)-rm -f *.dyn *.lock *.dpi main_st2.o main.o main.$(SUFFIX_BIN) main_phase1.$(SUFFIX_BIN) main_run.stamp
