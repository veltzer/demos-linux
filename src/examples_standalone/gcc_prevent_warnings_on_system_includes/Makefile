include ../../../Makefile.mk

# variables
ALL:=demo.1.o demo.2.o
ifeq ($(DO_INTEL),1)
	ALL+=demo.3.o demo.4.o
endif

.PHONY: all
all: $(ALL)

demo.1.o: demo.cc
	$(info doing [$@])
	$(Q)g++ -Weffc++ $<  -I ../../include/ -I include -o $@
demo.2.o: demo.cc
	$(info doing [$@])
	$(Q)g++ -Weffc++ $<  -I ../../include/ -isystem include -o $@
demo.3.o: demo.cc
	$(info doing [$@])
	$(Q)icpc -w3 $<  -I ../../include/ -I/usr/include/i386-linux-gnu/c++/4.8 -I include -o $@
demo.4.o: demo.cc
	$(info doing [$@])
	$(Q)icpc -w3 $<  -I ../../include/ -I/usr/include/i386-linux-gnu/c++/4.8 -isystem include -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)
