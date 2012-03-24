# the -pg is to generate profile info...
main.exe: main.cc
	g++ -pg -O2 -o $@ $<
.PHONY: clean
clean:
	-rm main.exe
