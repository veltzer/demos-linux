ALL:=mainpg.$(SUFFIX_BIN) main.$(SUFFIX_BIN) main.objdump mainpg.objdump
CFLAGS:=-I../../include

.PHONY: all
all: $(ALL) 

main.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) $< -o $@

mainpg.$(SUFFIX_BIN): main.c
	$(Q)gcc $(CFLAGS) -pg $< -o $@

main.objdump: main.$(SUFFIX_BIN)
	$(Q)objdump -D $< > $@

mainpg.objdump: mainpg.$(SUFFIX_BIN)
	$(Q)objdump -D $< > $@

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL) gmon.out
