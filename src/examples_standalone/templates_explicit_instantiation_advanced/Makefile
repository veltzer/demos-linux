CXXFLAGS:=-I../../include -I.

.PHONY: all
all: bar.o zoo.o ins.o

bar.o: bar.cc
	g++ $(CXXFLAGS) -Wall -Werror -c -o bar.o bar.cc
zoo.o: zoo.cc
	g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o zoo.o zoo.cc
ins.o: ins.cc
	g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o ins.o ins.cc

.PHONY: nm
nm:
	nm -C bar.o
	nm -C zoo.o
	nm -C ins.o

.PHONY: clean
clean:
	-rm *.o
