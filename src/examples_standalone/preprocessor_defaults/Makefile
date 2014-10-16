include ../../../Makefile.mk

# variables
ALL:=defs_c.out defs_cc.out

# targets
.PHONY: all
all: $(ALL)

defs_c.out: $(MAKEFILE_LIST)
	$(info doing [$@])
	$(Q)echo | gcc -E -dM - > $@
defs_cc.out: $(MAKEFILE_LIST)
	$(info doing [$@])
	$(Q)echo | g++ -E -dM - > $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)

.PHONY: debug
debug:
	$(info MAKEFILE_LIST is $(MAKEFILE_LIST))
