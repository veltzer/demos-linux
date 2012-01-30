CPPFLAGS=-g -rdynamic -Wall -Werror -I.
LDFLAGS=
CXX=g++
SOURCES=$(shell find . -name "*.cc")

OBJECTS=$(SOURCES:.cc=.o)
PROGS=$(SOURCES:.cc=)
all: $(PROGS)

clean:
	-rm -f $(PROGS)

check:
	-grep "  " `find . -name "*.cpp" -or -name "*.h"`

check_files:
	-find . -mindepth 2 -and -type f -and -not -name "*.cpp" -and -not -name "*.h" -and -not -name "*.h" -and -not -name "*.txt" -and -not -name "*.conf" -and -not -name "*.ini" -and -not -name "*.sample"
