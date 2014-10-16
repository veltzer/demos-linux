include ../../../Makefile.mk

ifeq ($(DO_INTEL),1)
	ALL:=f1.o f2.o f3.o
else
	ALL:=
endif

# variables
.PHONY: all
all: $(ALL)
	$(info doing [$@])

f1.o f2.o f3.o: f1.cc f2.cc f3.cc
	$(info doing [$@])
	$(Q)-icpc -c *.cc
#icpc -c -ipo-separate *.cc

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.o
