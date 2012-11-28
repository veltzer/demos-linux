CXXFLAGS:=-I../../include -O2 -Wall -Werror

main.exe: main_st2.o
	$(Q)g++ -o $@ $<
main_st2.o: main.cc main.gcda
	$(Q)g++ $(CXXFLAGS) -fprofile-use -c -o main.o main.cc
	$(Q)mv main.o main_st2.o
main.gcda main_run.stamp: main_phase1.exe
	$(Q)./main_phase1.exe 1> /dev/null 2> /dev/null
	$(Q)touch main_run.stamp
main_phase1.exe: main.o
	$(Q)g++ -fprofile-generate -o $@ $<
main.o: main.cc
	$(Q)g++ $(CXXFLAGS) -fprofile-generate -c -o main.o main.cc
.PHONY: clean
clean:
	$(Q)-rm -f main_st2.o main.gcda main.o main.exe main_phase1.exe main_run.stamp
