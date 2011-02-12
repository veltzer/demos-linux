# GNU makefile to build the solutions.
#
# Fixes done on the solutions:
# 1. moved all solutions to .cc files (to use C++ constants, loop initialization and more).
# 2. fixed up the solutions (handle more error codes in return and more).
# 3. renamed some of the files (for consistant filenames to all solutions).
# 4. cleaned up the source code (standartization).
#
# This makefile:
# - Assumes only one source file per executable and compiles them all
# - Also does -Wall -Werror - so you better write strict code
# - Compiles with the C++ compiler for extra strictness
#
# 				Mark Veltzer

SOURCES=$(shell find . -name "*.cc")
TARGETS=$(basename $(SOURCES))
CXXFLAGS+=-g3 -Wall -Werror -lpthread

all: $(TARGETS)

clean:
	-rm $(TARGETS)
check:
	find . -mindepth 2 -and -type f -and \( -not -name "*.cc" -and -not -name "*.hh" \)
	-grep "main" */*.cc */*.hh | fgrep -v "int main(int argc,char** argv,char** envp)"
	-grep "define" */*.cc */*.hh
	-grep "  " */*.cc */*.hh
