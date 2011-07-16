out.exe: A.o use_a.o
	g++ -o $@ A.o use_a.o
A.o: A.cc
	g++ -c -g3 -o $@ $<
use_a.o: use_a.cc
	g++ -c -g3 -o $@ $<

.PHONY: clean
clean:
	-rm -f A.o use_a.o out.exe

.PHONY: dump
dump:
	objdump --syms -C use_a.o
