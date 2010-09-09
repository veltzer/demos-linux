.PHONY: all
all: main

main: data.o main.o
	g++ -o $@ $?

# rule about how to create object files from text files
%.o: %.txt
	objcopy --input binary --output elf32-i386 --binary-architecture i386 $< $@

.PHONY: clean
clean:
	-rm -f *.o main
