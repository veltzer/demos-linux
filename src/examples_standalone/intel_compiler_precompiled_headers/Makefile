PCHI:=hello.pchi
EXE:=a.out
COMPILER:=icpc -I/usr/include/i386-linux-gnu/c++/4.7

$(EXE) $(PCHI): hello.cc hello.hh
	$(Q)$(COMPILER) -pch-create $(PCHI) hello.cc -o $(EXE)

.PHONY: clean
clean:
	$(Q)-rm -f $(PCHI) $(EXE) test_opt.out test_noopt.out test.out *.pchi

.PHONY: test
test:
	$(Q)time $(COMPILER) goodbye.cc -I. -include hello.hh -pch-use $(PCHI) -o test_opt.out 
	$(Q)time $(COMPILER) goodbye.cc -I. -include hello.hh -o test_noopt.out

.PHONY: test_pch
test_pch:
	$(Q)$(COMPILER) -o test.out -pch /home/mark/tmp/test_icpc/goodbye.cc
