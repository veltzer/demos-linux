include ../../../Makefile.mk

OBJECTS:=mq.o main.o
CFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: main.elf
	$(info doing [$@])

main.elf: $(OBJECTS)
	$(info doing [$@])
	$(Q)gcc -o $@ $(OBJECTS) -lpthread

mq.o: mq.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(info doing [$@])
	$(Q)gcc $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)rm -f $(OBJECTS) main.elf
