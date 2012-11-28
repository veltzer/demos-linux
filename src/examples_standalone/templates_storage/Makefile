CXXFLAGS:=-Wall -Werror -I../../include

out.exe: A.o use_a.o
	$(Q)g++ -o $@ A.o use_a.o
A.o: A.cc
	$(Q)g++ $(CXXFLAGS) -c -g3 -o $@ $<
use_a.o: use_a.cc
	$(Q)g++ $(CXXFLAGS) -c -g3 -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f A.o use_a.o out.exe

.PHONY: dump
dump:
	$(Q)objdump --syms -C use_a.o
