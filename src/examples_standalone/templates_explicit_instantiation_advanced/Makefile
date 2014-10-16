include ../../../Makefile.mk

# variables
CXXFLAGS:=-Wall -Werror -I../../include -I.

.PHONY: all
all: bar.o zoo.o ins.o
	$(info doing [$@])
bar.o: bar.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c -o bar.o bar.cc
zoo.o: zoo.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -c -o zoo.o zoo.cc
ins.o: ins.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -c -o ins.o ins.cc

.PHONY: nm
nm:
	$(info doing [$@])
	$(Q)nm -C bar.o
	$(Q)nm -C zoo.o
	$(Q)nm -C ins.o

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.o
