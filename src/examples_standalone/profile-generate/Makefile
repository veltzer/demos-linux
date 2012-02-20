main.exe: main_st2.o
	g++ -o $@ $<
main_st2.o: main.cc main.gcda
	g++ -O2 -fprofile-use -c -o main.o main.cc -Wall -Werror
	mv main.o main_st2.o
main.gcda main_run.stamp: main_phase1.exe
	./main_phase1.exe
	touch main_run.stamp
main_phase1.exe: main.o
	g++ -fprofile-generate -o $@ $<
main.o: main.cc
	g++ -O2 -fprofile-generate -c -o main.o main.cc -Wall -Werror
.PHONY: clean
clean:
	-rm main_st2.o main.gcda main.o main.exe main_phase1.exe main_run.stamp
