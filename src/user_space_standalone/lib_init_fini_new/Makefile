ALL:=main.exe libfoo.so libbar.so

CXXFLAGS=-I../../include

main.exe: main.cc libfoo.so libbar.so
	g++ $(CXXFLAGS) -o $@ $< -L. -lfoo -lbar -lpthread
libfoo.so: foo.cc
	g++ $(CXXFLAGS) -shared -o $@ $<
libbar.so: bar.cc
	g++ $(CXXFLAGS) -shared -o $@ $<

.PHONY: clean
clean:
	-rm -f $(ALL) 
.PHONY: dump
dump:
	objdump -t libfoo.so
.PHONY: dump_private
dump_private:
	objdump -p libfoo.so
.PHONY: dump_all
dump_all:
	objdump -s libfoo.so
