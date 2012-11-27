ALL:=mainpg.exe main.exe main.objdump mainpg.objdump
CFLAGS:=-I../../include

.PHONY: all
all: $(ALL) 

main.exe: main.c
	gcc $(CFLAGS) $< -o $@

mainpg.exe: main.c
	gcc $(CFLAGS) -pg $< -o $@

main.objdump: main.exe
	objdump -D $< > $@

mainpg.objdump: mainpg.exe
	objdump -D $< > $@

.PHONY: clean
clean:
	-rm $(ALL) gmon.out
