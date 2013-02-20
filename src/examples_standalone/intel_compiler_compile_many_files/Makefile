.PHONY: all
all: f1.o f2.o f3.o

f1.o f2.o f3.o: f1.cc f2.cc f3.cc
	icpc -c *.cc
#icpc -c -ipo-separate *.cc

.PHONY: all
clean:
	rm -f *.o
