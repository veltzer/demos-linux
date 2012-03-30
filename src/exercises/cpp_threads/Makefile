CXXFLAGS=-O2 -Wall -Werror -I. -I../../include
LDFLAGS=-lpthread
CXX=g++
SOURCES=$(shell find . -name "*.cc")

OBJECTS:=$(SOURCES:.cc=.o)
PROG:=main.exe


.PHONY: all
all: $(PROG)

main.exe: main.o MyThread.o
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f $(PROG) $(OBJECTS)
