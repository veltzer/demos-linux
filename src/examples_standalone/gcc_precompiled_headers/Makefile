include ../../../Makefile.defs

# these flags actually turn out to be nothing on ubuntu 13.10 (mark)
ACE_FLAGS:=$(shell pkg-config --cflags ACE)
#CFLAGS:=-H -Winvalid-pch
CFLAGS:=-Winvalid-pch
SOURCES=$(shell find . -name "*.cc")
OBJECTS=$(shell ./objects.sh)

.PHONY: all
all: pch/ace.hh.gch $(OBJECTS)

pch/ace.hh.gch: include/ace.hh
	$(info doing [$@])
	$(Q)g++ -Iinclude $(CFLAGS) $(ACE_FLAGS) -o $@ $<

# rules
%.precompiled.o:  main.cc
	$(info doing [$@])
	$(Q)g++ -Ipch $(CFLAGS) $(ACE_FLAGS) -c -o $@ $<
%.noprecomp.o: main.cc
	$(info doing [$@])
	$(Q)g++ -Iinclude $(CFLAGS) $(ACE_FLAGS) -c -o $@ $<

.PHONY: debug
debug:
	$(info ACE_FLAGS is $(ACE_FLAGS))
	$(info SOURCES is $(SOURCES))
	$(info OBJECTS is $(OBJECTS))

.PHONY: clean
clean:
	$(Q)-rm -f pch/ace.hh.gch *.o
