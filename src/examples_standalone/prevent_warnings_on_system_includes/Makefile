ALL:=demo.1.o demo.2.o demo.3.o demo.4.o

.PHONY: all
all: $(ALL)

demo.1.o: demo.cc
	icpc -w3 $<  -I ../../include/ -I/usr/include/i386-linux-gnu/c++/4.8 -I include -o $@
demo.2.o: demo.cc
	icpc -w3 $<  -I ../../include/ -I/usr/include/i386-linux-gnu/c++/4.8 -isystem include -o $@
demo.3.o: demo.cc
	g++ -Weffc++ $<  -I ../../include/ -I include -o $@
demo.4.o: demo.cc
	g++ -Weffc++ $<  -I ../../include/ -isystem include -o $@

.PHONY: clean
clean:
	rm -f $(ALL)
