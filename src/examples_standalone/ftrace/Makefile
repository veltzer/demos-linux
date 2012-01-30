ALL:=mainpg main main.objdump mainpg.objdump
.PHONY: all
all: $(ALL) 

main: main.c
	gcc $< -o $@

mainpg: main.c
	gcc -pg $< -o $@

main.objdump: main
	objdump -D $< > $@

mainpg.objdump: mainpg
	objdump -D $< > $@

.PHONY: clean
clean:
	-rm $(ALL) gmon.out
