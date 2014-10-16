include ../../../Makefile.mk

ALL:=main1.o
ifeq ($(DO_INTEL),1)
	ALL+=main3.o main4.o
endif

.PHONY: all
all: $(ALL)

main1.o: main.c
	$(info doing [$@])
	$(Q)gcc -c main.c -o main1.o -Ifolder2 -Ifolder1

main2.o: main.c
	$(info doing [$@])
	$(Q)gcc -c main.c -iquote folder2 -iquote folder1

main3.o: main.c
	$(info doing [$@])
	$(Q)icpc -c main.c -Ifolder2 -Ifolder1

main4.o: main.c
	$(info doing [$@])
	$(Q)icpc -c main.c -I- -Ifolder2 -Ifolder1

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)rm -f *.o
