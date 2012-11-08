# variables
CXX:=icc
#CXXFLAGS:=-I/usr/include/i386-linux-gnu
CXXFLAGS:=-Wall -Werror -O2
PROF_GEN=-prof-gen
PROF_USE=-prof-use

# targets
main.exe: main_st2.o
	$(CXX) -o $@ $<
main_st2.o: main.cc main.gcda
	$(CXX) $(CXXFLAGS) $(PROF_USE) -c -o main.o main.cc
	mv main.o main_st2.o
main.gcda main_run.stamp: main_phase1.exe
	./main_phase1.exe
	touch main_run.stamp
main_phase1.exe: main.o
	$(CXX) -o $@ $<
main.o: main.cc
	$(CXX) $(CXXFLAGS) $(PROF_GEN) -c -o main.o main.cc
.PHONY: clean
clean:
	-rm *.dyn *.lock *.dpi main_st2.o main.o main.exe main_phase1.exe main_run.stamp
