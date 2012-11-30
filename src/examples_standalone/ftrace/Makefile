ALL:=mainpg.exe main.exe main.objdump mainpg.objdump
CFLAGS:=-I../../include

.PHONY: all
all: $(ALL) 

main.exe: main.c
	$(Q)gcc $(CFLAGS) $< -o $@

mainpg.exe: main.c
	$(Q)gcc $(CFLAGS) -pg $< -o $@

main.objdump: main.exe
	$(Q)objdump -D $< > $@

mainpg.objdump: mainpg.exe
	$(Q)objdump -D $< > $@

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL) gmon.out
