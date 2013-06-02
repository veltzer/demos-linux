include ../../../Makefile.defs
# variables
CXX:=icpc
CXXFLAGS:=-Wall -Werror -I../../include -I/usr/include/i386-linux-gnu/c++/4.7
ALL:=ver0.o ver1.o ver2.o ver3.o ver4.o ver5.o ver6.o ver7.o ver8.o ver9.o

# targets
.PHONY: all
all: $(ALL)

ver0.o: debug_size.cc
	$(Q)$(CXX) -c $(CXXFLAGS) $< -o $@
ver1.o: debug_size.cc
	$(Q)$(CXX) -g -c $(CXXFLAGS) $< -o $@
ver2.o: debug_size.cc
	$(Q)$(CXX) -g2 -c $(CXXFLAGS) $< -o $@
ver3.o: debug_size.cc
	$(Q)$(CXX) -g3 -c $(CXXFLAGS) $< -o $@
ver4.o: debug_size.cc
	$(Q)$(CXX) -g -femit-struct-debug-baseonly -c $(CXXFLAGS) $< -o $@
ver5.o: debug_size.cc
	$(Q)$(CXX) -g -O2 -c $(CXXFLAGS) $< -o $@
ver6.o: debug_size.cc
	$(Q)$(CXX) -O2 -g -c $(CXXFLAGS) $< -o $@
ver7.o: debug_size.cc
	$(Q)$(CXX) -debug none -O2 -c $(CXXFLAGS) $< -o $@
ver8.o: debug_size.cc
	$(Q)$(CXX) -debug all -O2 -c $(CXXFLAGS) $< -o $@
ver9.o: debug_size.cc
	$(Q)$(CXX) -debug minimal -O2 -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL)
