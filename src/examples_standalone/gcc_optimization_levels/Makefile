include ../../../Makefile.mk

ALL:=O0-opts.out O1-opts.out O2-opts.out O3-opts.out O4-opts.out

.PHONY: all
all: $(ALL)
	$(info doing [$@])

O0-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O0 --help=optimizers > $@
O1-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O1 --help=optimizers > $@
O2-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O2 --help=optimizers > $@
O3-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O3 --help=optimizers > $@
O4-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O4 --help=optimizers > $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)

.PHONY: count
count:
	$(info doing [$@])
	$(Q)for x in *.out; do echo $$x; grep "enabled" $$x | wc -l;done
