CXXFLAGS=-g -rdynamic -Wall -Werror -I../../include
LDFLAGS=-ldl

.PHONY: all
all: $(PROGS)

run_func_from_dll: run_func_from_dll.cc
	$(CXX) $(CXXFLAGS) $@ -o $< $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f run_func_from_dll

.PHONY: check
check:
	-grep "\ \ " `find . -name "*.cpp" -or -name "*.h"`

.PHONY: check_files
check_files:
	-find . -mindepth 2 -and -type f -and -not -name "*.cpp" -and -not -name "*.h" -and -not -name "*.h" -and -not -name "*.txt" -and -not -name "*.conf" -and -not -name "*.ini" -and -not -name "*.sample"
