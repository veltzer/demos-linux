include ../../../Makefile.defs

CFLAGS:=-I../../include

.PHONY: all
all: main.$(SUFFIX_BIN) libadd.so

main.$(SUFFIX_BIN): main.o 
	$(info doing [$@])
	$(Q)gcc -o $@ $< -ldl
main.o: main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<
libadd.so: add.o
	$(info doing [$@])
	$(Q)gcc -shared -o $@ $<
add.o: add.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.o main.$(SUFFIX_BIN) libadd.so *.stamp
