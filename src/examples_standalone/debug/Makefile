CFLAGS:=-I../../include

.PHONY:all
all: main.forclient.exe main.forhq.exe

main.forclient.exe: main.c
	$(Q)gcc $(CFLAGS) -s -g3 -o $@ $<
main.forhq.exe: main.c
	$(Q)gcc $(CFLAGS) -g3 -o $@ $<
clean:
	$(Q)-rm -f main.forclient.exe main.forhq.exe core
