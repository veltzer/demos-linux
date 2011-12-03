.PHONY: all
all: O-opts.txt O2-opts.txt O3-opts.txt

.PHONY: clean
clean:
	-rm -f O-opts.txt O2-opts.txt O3-opts.txt

O-opts.txt O2-opts.txt O3-opts.txt:
	gcc -Q -O --help=optimizers > O-opts.txt
	gcc -Q -O2 --help=optimizers > O2-opts.txt
	gcc -Q -O3 --help=optimizers > O3-opts.txt
