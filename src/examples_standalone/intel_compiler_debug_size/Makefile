include ../../../Makefile.mk

# variables
CXX:=icpc
CXXFLAGS:=-Wall -Werror -I../../include -I/usr/include/i386-linux-gnu/c++/4.8
ALL:=ver0.o ver1.o ver2.o ver3.o ver4.o ver5.o ver6.o ver7.o ver8.o ver9.o

# targets
.PHONY: all
all: $(ALL)

ver0.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -c $(CXXFLAGS) $< -o $@
ver1.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -g -c $(CXXFLAGS) $< -o $@
ver2.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -g2 -c $(CXXFLAGS) $< -o $@
ver3.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -g3 -c $(CXXFLAGS) $< -o $@
ver4.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -g -O2 -c $(CXXFLAGS) $< -o $@
ver5.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -O2 -g -c $(CXXFLAGS) $< -o $@
ver6.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -debug none -O2 -c $(CXXFLAGS) $< -o $@
ver7.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -debug all -O2 -c $(CXXFLAGS) $< -o $@
ver8.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -debug minimal -O2 -c $(CXXFLAGS) $< -o $@
ver9.o: big_debuginfo_file.cc
	$(info doing [$@])
	$(Q)$(CXX) -g -debug noinline-debug-info -O2 -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)
