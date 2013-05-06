.PHONY: all
all: O-opts.out O2-opts.out O3-opts.out

.PHONY: clean
clean:
	-rm -f O-opts.out O2-opts.out O3-opts.out

O-opts.out O2-opts.out O3-opts.out:
	gcc -Q -O --help=optimizers > O-opts.out
	gcc -Q -O2 --help=optimizers > O2-opts.out
	gcc -Q -O3 --help=optimizers > O3-opts.out
