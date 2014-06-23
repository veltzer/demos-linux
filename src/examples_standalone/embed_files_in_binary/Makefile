include ../../../Makefile.mk
CXXFLAGS:=-I../../include

ARCH:=i386
# use objcopy --info to see all supported output formats
GCC_MACHINE:=$(shell gcc -dumpmachine)
ifeq ($(GCC_MACHINE),x86_64-linux-gnu)
	OUT_FORMAT:=elf64-x86-64
else
	OUT_FORMAT:=elf32-i386
endif

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): data.o main.o
	$(Q)g++ -o $@ data.o main.o

main.o: main.cc
	$(Q)g++ $(CXXFLAGS) -c -o $@ $<

# rule about how to create object files from text files
%.o: %.txt
	$(Q)objcopy --input-format binary --output-format $(OUT_FORMAT) --binary-architecture $(ARCH) $< $@

.PHONY: clean
clean:
	$(Q)-rm -f *.o main.$(SUFFIX_BIN)
