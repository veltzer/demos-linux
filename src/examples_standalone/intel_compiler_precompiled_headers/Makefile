PCHI:=hello.pchi
EXE:=a.out

$(EXE) $(PCHI): hello.cc hello.hh
	$(Q)icpc -pch-create $(PCHI) hello.cc -o $(EXE)

.PHONY: clean
clean:
	$(Q)-rm -f $(PCHI) $(EXE) test_opt.out test_noopt.out test.out *.pchi

.PHONY: test
test:
	$(Q)time icpc goodbye.cc -I. -include hello.hh -pch-use $(PCHI) -o test_opt.out 
	$(Q)time icpc goodbye.cc -I. -include hello.hh -o test_noopt.out

.PHONY: test_pch
test_pch:
	$(Q)icpc -o test.out -pch /home/mark/tmp/test_icpc/goodbye.cc
