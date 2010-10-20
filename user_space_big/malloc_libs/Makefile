ALL:=memory.dmalloc.exe memory.regular.exe memory.efence.exe free.regular.exe free.dmalloc.exe free.efence.exe
all: $(ALL) 

memory.regular.exe: memory.cc
	g++ -g3 -o $@ $<
memory.efence.exe: memory.cc
	g++ -g3 -o $@ $< -lefence
memory.dmalloc.exe: memory.cc
	g++ -g3 -o $@ $< -ldmalloc
free.regular.exe: free.cc
	g++ -g3 -o $@ $<
free.dmalloc.exe: free.cc
	g++ -g3 -DDMALLOC -o $@ $< -ldmalloc
free.efence.exe: free.cc
	g++ -g3 -o $@ $< -lefence
.PHONY: clean
clean:
	-rm -f $(ALL) 
