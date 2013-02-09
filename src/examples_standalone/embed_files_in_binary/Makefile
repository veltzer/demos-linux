CXXFLAGS:=-I../../include

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): data.o main.o
	$(Q)g++ -o $@ $?

main.o: main.cc
	$(Q)g++ $(CXXFLAGS) -c -o $@ $<

# rule about how to create object files from text files
%.o: %.txt
	$(Q)objcopy --input binary --output elf32-i386 --binary-architecture i386 $< $@

.PHONY: clean
clean:
	$(Q)-rm -f *.o main.$(SUFFIX_BIN)
