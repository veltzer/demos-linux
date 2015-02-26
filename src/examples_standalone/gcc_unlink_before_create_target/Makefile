include ../../../Makefile.mk

CXXFLAGS:=-I../../include -O2 -Wall -Werror

.PHONY: all
all:
	$(Q)g++ -c $(CXXFLAGS) -o main1.o main1.cc
	$(Q)rm -f main2.o
	$(Q)ln main1.o main2.o
	$(Q)echo before the second compile...
	$(Q)ls -l *.o
	$(Q)strace -f g++ -c $(CXXFLAGS) -o main2.o main2.cc 2> log.out
	$(Q)echo after the second compile...
	$(Q)ls -l *.o
	$(Q)echo here is an strace on what the compiler did...
	$(Q)grep main2.o log.out | tail -n 2

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main1.o main2.o log.out
