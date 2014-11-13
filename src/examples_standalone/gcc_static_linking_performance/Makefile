include ../../../Makefile.mk

CXXFLAGS:=-I. -I../../include -O2 -Wall -Werror
ALL:=\
	something.o\
	libsomething.a\
	libsomething.so\
	static.$(SUFFIX_BIN)\
	dynamic.$(SUFFIX_BIN)

.PHONY: all
all: $(ALL)
	$(info doing [$@])

static.$(SUFFIX_BIN): main.cc libsomething.a
	$(info doing [$@])
	$(Q)g++ -static $(CXXFLAGS) $^ -o $@ -L. -lsomething

dynamic.$(SUFFIX_BIN): main.cc libsomething.so
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $^ -o $@ -L. -lsomething

libsomething.so: something.cc
	$(info doing [$@])
	$(Q)g++ -shared $(CXXFLAGS) $^ -o $@

libsomething.a: something.o
	$(info doing [$@])
	$(Q)ar rcs $@ $^

# this is the static object, no need for -fpic
something.o: something.cc
	$(info doing [$@])
	$(Q)g++ -c $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)

.PHONY: time
time:
	$(info doing [$@])
	$(Q)echo measureing dynamic
	$(Q)LD_LIBRARY_PATH=. time -p ./dynamic.elf
	$(Q)echo measureing static
	$(Q)time -p ./static.elf
