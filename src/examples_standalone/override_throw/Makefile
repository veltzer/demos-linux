include ../../../Makefile.mk

ALL:=main.$(SUFFIX_BIN) main_linked.$(SUFFIX_BIN) libthrow.so

.PHONY: all
all: $(ALL)
	$(info doing [$@])
libthrow.so: cxa_throw_replace_backtrace.c
	$(info doing [$@])
	$(Q)gcc -shared -fPIC -o $@ $< -ldl
main.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ -o $@ main.cc
main_linked.$(SUFFIX_BIN): main.cc libthrow.so
	$(info doing [$@])
	$(Q)g++ -o $@ main.cc -L. -lthrow
.PHONY: run
run: main.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)./main.$(SUFFIX_BIN)
.PHONY: run_linked
run_linked: main_linked.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)LD_LIBRARY_PATH=. ./main_linked.$(SUFFIX_BIN)
.PHONY: run_preload
run_preload: main.$(SUFFIX_BIN) libthrow.so
	$(info doing [$@])
	$(Q)LD_PRELOAD=$(CURDIR)/libthrow.so ./main.$(SUFFIX_BIN)
.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)
