include ../../../Makefile.defs

CFLAGS:=-Wall -Werror -I../../include

prog.$(SUFFIX_BIN): time.o main.o
	$(info doing [$@])
	$(Q)g++ -o $@ time.o main.o
time.o: time.cc
	$(info doing [$@])
	$(Q)g++ $(CFLAGS) -c -o $@ $<
main.o: main.cc
	$(info doing [$@])
	$(Q)g++ $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o prog.$(SUFFIX_BIN)
