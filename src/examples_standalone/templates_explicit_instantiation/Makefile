include ../../../Makefile.defs

CXXFLAGS:=-I../../include -Wall -Werror -fno-implicit-inline-templates -fno-implicit-templates
OBJECTS:=bar.o zoo.o ins.o

.phony: all
all: out.$(SUFFIX_BIN)

out.$(SUFFIX_BIN): $(OBJECTS)
	$(info doing [$@])
	$(Q)g++ $(OBJECTS) -o $@

bar.o: bar.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -o bar.o bar.cc
zoo.o: zoo.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -o zoo.o zoo.cc
ins.o: ins.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -o ins.o ins.cc

.PHONY: nm
nm:
	$(info doing [$@])
	$(Q)nm -C bar.o
	$(Q)nm -C zoo.o
	$(Q)nm -C ins.o

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(OBJECTS) out.$(SUFFIX_BIN) *.stamp
