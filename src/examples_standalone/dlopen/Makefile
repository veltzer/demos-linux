CFLAGS:=-I../../include

.PHONY: all
all: main.exe libadd.so

main.exe: main.o 
	$(Q)$(CC) -o $@ $< -ldl
main.o: main.c
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< 
libadd.so: add.o
	$(Q)$(CC) -shared -o $@ $<
add.o: add.c
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o main.exe libadd.so
