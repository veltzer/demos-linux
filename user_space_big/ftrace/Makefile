all: main.objdump mainpg.objdump

main: main.c
	gcc $< -o $@

mainpg: main.c
	gcc -pg $< -o $@

main.objdump: main
	objdump -D $< > $@

mainpg.objdump: mainpg
	objdump -D $< > $@

clean:
	-rm mainpg main main.objdump mainpg.objdump gmon.out
