CFLAGS:=-Wall -Werror -I../../include

prog.$(SUFFIX_BIN): main.o add.o
	$(Q)gcc -o $@ main.o add.o 
main.o: main.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<
add.o: add.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o prog.$(SUFFIX_BIN)
