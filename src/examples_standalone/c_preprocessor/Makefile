.PHONY: all
all: debug.exe release.exe

debug.exe: main.c
	gcc $< -DTYPE=DEBUG -o $@
release.exe: main.c
	gcc $< -DTYPE=RELEASE -o $@

.PHONY: clean
clean:
	rm -f *.exe
