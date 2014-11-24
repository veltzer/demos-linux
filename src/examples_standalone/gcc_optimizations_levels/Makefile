include ../../../Makefile.mk

ALL:=O-opts.out O2-opts.out O3-opts.out

.PHONY: all
all: $(ALL)
	$(info doing [$@])

O-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O --help=optimizers > $@
O2-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O2 --help=optimizers > $@
O3-opts.out:
	$(info doing [$@])
	$(Q)gcc -Q -O3 --help=optimizers > $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)
