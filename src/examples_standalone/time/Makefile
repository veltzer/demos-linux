CFLAGS:=-Wall -Werror -I../../include

prog.exe: time.o main.o
	$(Q)g++ -o $@ time.o main.o
time.o: time.cc
	$(Q)g++ $(CFLAGS) -c -o $@ $<
main.o: main.cc
	$(Q)g++ $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f *.o prog.exe
