ALL:=memory_error.dmalloc.exe memory_error.regular.exe memory_error.efence.exe

ALL_DEPS:=Makefile

.PHONY: all
all: $(ALL) 

memory_error.regular.exe: memory_error.cc $(ALL_DEPS)
	g++ -O2 -g3 -o $@ $<
memory_error.efence.exe: memory_error.cc $(ALL_DEPS)
	g++ -O2 -g3 -o $@ $< -lefence
memory_error.dmalloc.exe: memory_error.cc $(ALL_DEPS)
	g++ -O2 -g3 -DDMALLOC -o $@ $< -ldmalloc
.PHONY: clean
clean:
	-rm -f $(ALL) logfile.out
