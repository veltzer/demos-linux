include ../../../Makefile.mk

ALL:=file1.$(SUFFIX_O) file2.$(SUFFIX_O) file3.$(SUFFIX_O)

.PHONY: all
all: all.stamp

all.stamp: file1.c file2.c file3.c
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -c file1.c file2.c file3.c
	$(Q)touch $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) all.stamp
