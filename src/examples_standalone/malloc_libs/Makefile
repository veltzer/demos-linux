ALL:=memory_error.dmalloc.exe memory_error.regular.exe memory_error.efence.exe
ALL_DEPS:=Makefile
CXXFLAGS:=-O2 -g3 -Wall -Werror -I../../include

.PHONY: all
all: $(ALL) 

memory_error.regular.exe: memory_error.cc $(ALL_DEPS)
	$(Q)g++ $(CXXFLAGS) -o $@ $<
memory_error.efence.exe: memory_error.cc $(ALL_DEPS)
	$(Q)g++ $(CXXFLAGS) -o $@ $< -lefence
memory_error.dmalloc.exe: memory_error.cc $(ALL_DEPS)
	$(Q)g++ $(CXXFLAGS) -DDMALLOC -o $@ $< -ldmalloc
.PHONY: clean
clean:
	$(Q)-rm -f $(ALL) logfile.out
.PHONY: valgrind_memcheck
valgrind_memcheck:
	$(Q)valgrind --tool=memcheck ./memory_error.regular.exe 0
