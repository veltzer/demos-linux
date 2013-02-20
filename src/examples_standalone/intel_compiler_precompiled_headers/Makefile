PCHI:=hello.pchi
EXE:=a.out

$(EXE) $(PCHI): hello.cc hello.h
	icpc -pch-create $(PCHI) hello.cc -o $(EXE)

.PHONY: clean
clean:
	rm -f $(PCHI) $(EXE) test_opt.out test_noopt.out test.out *.pchi

.PHONY: test
test:
	time icpc goodbye.cc -I. -include hello.h -pch-use $(PCHI) -o test_opt.out 
	time icpc goodbye.cc -I. -include hello.h -o test_noopt.out

.PHONY: test_pch
test_pch:
	icpc -o test.out -pch /home/mark/tmp/test_icpc/goodbye.cc
