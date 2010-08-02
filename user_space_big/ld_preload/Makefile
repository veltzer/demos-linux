ALL:=main.exe mainlib1.exe mainlib2.exe liblib1.so liblib2.so libm.so.6
CXXFLAGS=-I../../include

.PHONY: all
all: $(ALL)

mainlib2.exe: main.cc liblib2.so
	g++ $(CXXFLAGS) -o $@ $< -L. -llib2
mainlib1.exe: main.cc liblib1.so
	g++ $(CXXFLAGS) -o $@ $< -L. -llib1
main.exe: main.cc
	g++ $(CXXFLAGS) -o $@ $< -lm
liblib1.so: lib1.cc
	g++ $(CXXFLAGS) -fpic -shared -o $@ $<
liblib2.so: lib2.cc
	g++ $(CXXFLAGS) -fpic -shared -o $@ $< -ldl
libm.so.6: lib2.cc
	g++ $(CXXFLAGS) -fpic -shared -o $@ $< -ldl

.PHONY: clean
clean:
	-rm -f $(ALL) *.dis

VAL=0.5
# this simply runs the good old executable
.PHONY: run
run:
	./main.exe $(VAL)
# this simply runs the good old executable with lib1 preloaded
.PHONY: run_ldprelib1
run_ldprelib1:
	LD_PRELOAD=./liblib1.so ./main.exe $(VAL)
# this time with libm.so.6 preloaded
.PHONY: run_ldprem
run_ldprem:
	LD_PRELOAD=./libm.so.6 ./main.exe $(VAL)
# just library path changes
.PHONY: run_ldlibpath
run_ldlibpath:
	LD_LIBRARY_PATH=. ./main.exe $(VAL)

# rule about how to create disassembly files...
%.dis: %.exe
	objdump --source --disassemble $< > $@
