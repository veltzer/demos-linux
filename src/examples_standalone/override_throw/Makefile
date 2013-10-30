include ../../../Makefile.defs

.PHONY: all
all: main.$(SUFFIX_BIN) libthrow.so

libthrow.so: cxa_throw_replace_backtrace.c
	$(info doing [$@])
	$(Q)gcc -shared -fPIC -o $@ $< -ldl

main.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ -o $@ main.cc

.PHONY: run
run: main.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)./main.$(SUFFIX_BIN)
.PHONY: run_lib
run_lib: main.$(SUFFIX_BIN) libthrow.so
	$(info doing [$@])
	$(Q)LD_PRELOAD=$(CURDIR)/libthrow.so ./main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN) libthrow.so
