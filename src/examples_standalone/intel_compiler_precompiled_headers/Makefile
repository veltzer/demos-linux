PCHI:=hello.pchi
EXE:=a.out

GCC_MACHINE:=$(shell gcc -dumpmachine)
ifeq ($(GCC_MACHINE),x86_64-linux-gnu)
	ARCH:=x86_64
else
	ARCH:=i386
endif
FOLDER:=/usr/include/$(ARCH)-linux-gnu/c++/4.8
CXX:=icpc
CXXFLAGS=-I$(FOLDER) -I.

$(EXE) $(PCHI): hello.cc hello.hh
	$(Q)$(CXX) $(CXXFLAGS) -pch-create $(PCHI) hello.cc -o $(EXE)

.PHONY: clean
clean:
	$(Q)-rm -f $(PCHI) $(EXE) test_opt.out test_noopt.out test.out *.pchi

.PHONY: test
test:
	$(Q)time $(CXX) $(CXXFLAGS) -pch-use $(PCHI) goodbye.cc -o test_opt.out 
	$(Q)time $(CXX) $(CXXFLAGS) goodbye.cc -o test_noopt.out

.PHONY: test_pch
test_pch:
	$(Q)$(COMPILER) -o test.out -pch /home/mark/tmp/test_icpc/goodbye.cc
