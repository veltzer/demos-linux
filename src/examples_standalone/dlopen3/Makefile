CFLAGS:=-I../../include

.PHONY: all
all: main.$(SUFFIX_BIN) libadd.so

main.$(SUFFIX_BIN): main.o 
	$(Q)gcc -o $@ $< -ldl
main.o: main.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<
libadd.so: add.o
	$(Q)gcc -shared -o $@ $<
add.o: add.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o main.$(SUFFIX_BIN) libadd.so
