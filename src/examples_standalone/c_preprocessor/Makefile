CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: debug.exe release.exe

debug.exe: main.c
	gcc $(CFLAGS) $< -DTYPE=DEBUG -o $@
release.exe: main.c
	gcc $(CFLAGS) $< -DTYPE=RELEASE -o $@

.PHONY: clean
clean:
	rm -f *.exe
