CXXFLAGS:=-I../../include -O2 -Wall -Werror

main.exe: main_st2.o
	g++ -o $@ $<
main_st2.o: main.cc main.gcda
	g++ $(CXXFLAGS) -fprofile-use -c -o main.o main.cc
	mv main.o main_st2.o
main.gcda main_run.stamp: main_phase1.exe
	./main_phase1.exe
	touch main_run.stamp
main_phase1.exe: main.o
	g++ -fprofile-generate -o $@ $<
main.o: main.cc
	g++ $(CXXFLAGS) -fprofile-generate -c -o main.o main.cc
.PHONY: clean
clean:
	-rm main_st2.o main.gcda main.o main.exe main_phase1.exe main_run.stamp
