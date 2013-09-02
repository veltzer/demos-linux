include ../../../Makefile.defs

ALL:=mainpg.$(SUFFIX_BIN) main.$(SUFFIX_BIN) main.objdump mainpg.objdump
CFLAGS:=-I../../include

.PHONY: all
all: $(ALL) 

main.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) $< -o $@

mainpg.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -pg $< -o $@

main.objdump: main.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)objdump -D $< > $@

mainpg.objdump: mainpg.$(SUFFIX_BIN)
	$(info doing [$@])
	$(Q)objdump -D $< > $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL) gmon.out *.stamp
