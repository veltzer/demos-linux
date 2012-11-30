CFLAGS:=-I../../include

.PHONY: all
all: main.exe libadd.so

main.exe: main.o 
	$(Q)gcc -o $@ $< -ldl
main.o: main.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<
libadd.so: add.o
	$(Q)gcc -shared -o $@ $<
add.o: add.c
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o main.exe libadd.so
