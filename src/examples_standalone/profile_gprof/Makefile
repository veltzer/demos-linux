# the -pg is to generate profile info...
main.exe: main.cc
	$(Q)g++ -pg -I../../include -O2 -o $@ $<
.PHONY: clean
clean:
	$(Q)-rm -f main.exe
