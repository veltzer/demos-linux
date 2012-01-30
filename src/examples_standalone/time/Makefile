prog: time.o main.o
	g++ -o prog time.o main.o
time.o: time.cc
	g++ -c -o time.o time.cc
main.o: main.cc
	g++ -c -o main.o main.cc

.PHONY: clean
clean:
	-rm -f *.o prog
