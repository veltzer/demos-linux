include ../../../Makefile.defs

ALL:=main.$(SUFFIX_BIN) libfoo.so libbar.so
CXXFLAGS:=-I../../include
SHARED_FLAGS:=-shared -fPIC

.PHONY: all
all: main.$(SUFFIX_BIN)
	$(info doing [$@])

main.$(SUFFIX_BIN): main.cc libfoo.so libbar.so
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) -o $@ $< -L. -lfoo -lbar -lpthread
libfoo.so: foo.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $<
libbar.so: bar.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $(SHARED_FLAGS) -o $@ $<

.PHONY: run
run: main.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)LD_LIBRARY_PATH=. ./main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) 
.PHONY: dump
dump:
	$(info doing [$@])
	$(Q)objdump -t libfoo.so
.PHONY: dump_private
dump_private:
	$(info doing [$@])
	$(Q)objdump -p libfoo.so
.PHONY: dump_all
dump_all:
	$(info doing [$@])
	$(Q)objdump -s libfoo.so
