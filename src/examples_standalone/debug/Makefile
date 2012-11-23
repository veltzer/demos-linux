CFLAGS:=-I../../include

.PHONY:all
all: main.forclient.exe main.forhq.exe

main.forclient.exe: main.c
	gcc $(CFLAGS) -s -g3 -o $@ $<
main.forhq.exe: main.c
	gcc $(CFLAGS) -g3 -o $@ $<
clean:
	-rm -f main.forclient.exe main.forhq.exe core
