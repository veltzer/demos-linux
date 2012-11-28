ALL:=main.exe libfoo.so libbar.so

CXXFLAGS=-I../../include

main.exe: main.cc libfoo.so libbar.so
	$(Q)g++ $(CXXFLAGS) -o $@ $< -L. -lfoo -lbar -lpthread
libfoo.so: foo.cc
	$(Q)g++ $(CXXFLAGS) -shared -o $@ $<
libbar.so: bar.cc
	$(Q)g++ $(CXXFLAGS) -shared -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL) 
.PHONY: dump
dump:
	$(Q)objdump -t libfoo.so
.PHONY: dump_private
dump_private:
	$(Q)objdump -p libfoo.so
.PHONY: dump_all
dump_all:
	$(Q)objdump -s libfoo.so
