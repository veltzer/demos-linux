CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: debug.exe release.exe

debug.exe: main.c
	$(Q)gcc $(CFLAGS) $< -DTYPE=DEBUG -o $@
release.exe: main.c
	$(Q)gcc $(CFLAGS) $< -DTYPE=RELEASE -o $@

.PHONY: clean
clean:
	$(Q)rm -f *.exe
