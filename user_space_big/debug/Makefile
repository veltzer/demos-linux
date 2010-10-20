all: main.forclient main.forhq

main.forclient: main.c
	gcc -s -g3 -o $@ main.c
main.forhq: main.c
	gcc -g3 -o $@ main.c
clean:
	-rm -f main.forclient main.forhq core
