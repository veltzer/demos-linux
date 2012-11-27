CFLAGS:=-Wall -Werror -I../../include

prog.exe: time.o main.o
	g++ -o $@ time.o main.o
time.o: time.cc
	g++ $(CFLAGS) -c -o $@ $<
main.o: main.cc
	g++ $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm -f *.o prog.exe
