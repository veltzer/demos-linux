ALL:=main.$(SUFFIX_BIN) mainlib1.$(SUFFIX_BIN) mainlib2.$(SUFFIX_BIN) liblib1.so liblib2.so libm.so
CXXFLAGS=-I../../include

.PHONY: all
all: $(ALL)

mainlib1.$(SUFFIX_BIN): main.cc liblib1.so
	$(Q)g++ $(CXXFLAGS) -o $@ $< -L. -llib1
mainlib2.$(SUFFIX_BIN): main.cc liblib2.so
	$(Q)g++ $(CXXFLAGS) -o $@ $< -L. -llib2
main.$(SUFFIX_BIN): main.cc
	$(Q)g++ $(CXXFLAGS) -o $@ $< -lm
liblib1.so: lib1.cc
	$(Q)g++ $(CXXFLAGS) -fpic -shared -o $@ $<
liblib2.so: lib2.cc
	$(Q)g++ $(CXXFLAGS) -fpic -shared -o $@ $< -ldl
libm.so: lib2.cc
	$(Q)g++ $(CXXFLAGS) -fpic -shared -o $@ $< -ldl

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL) *.dis

VAL=0.5
# this simply runs the good old $(SUFFIX_BIN)cutable
.PHONY: run
run:
	./main.$(SUFFIX_BIN) $(VAL)
# this simply runs the good old $(SUFFIX_BIN)cutable with lib1 preloaded
.PHONY: run_ldprelib1
run_ldprelib1:
	LD_PRELOAD=./liblib1.so ./main.$(SUFFIX_BIN) $(VAL)
# this time with libm.so.6 preloaded
.PHONY: run_ldprem
run_ldprem:
	LD_PRELOAD=./libm.so.6 ./main.$(SUFFIX_BIN) $(VAL)
# just library path changes
.PHONY: run_ldlibpath
run_ldlibpath:
	LD_LIBRARY_PATH=. ./main.$(SUFFIX_BIN) $(VAL)

# rule about how to create disassembly files...
%.dis: %.$(SUFFIX_BIN)
	objdump --source --disassemble $< > $@
