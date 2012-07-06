CXXFLAGS:=-I../../include -Wall -Werror -fno-implicit-inline-templates -fno-implicit-templates
OBJECTS:=bar.o zoo.o ins.o

.phony: all
all: out.exe

out.exe: $(OBJECTS)
	g++ $(OBJECTS) -o $@

bar.o: bar.cc
	g++ $(CXXFLAGS) -c -o bar.o bar.cc
zoo.o: zoo.cc
	g++ $(CXXFLAGS) -c -o zoo.o zoo.cc
ins.o: ins.cc
	g++ $(CXXFLAGS) -c -o ins.o ins.cc

.PHONY: nm
nm:
	nm -C bar.o
	nm -C zoo.o
	nm -C ins.o

.PHONY: clean
clean:
	-rm $(OBJECTS) out.exe
