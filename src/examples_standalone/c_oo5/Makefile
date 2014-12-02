include ../../../Makefile.mk

OBJECTS:=main.o rectangle.o
CFLAGS:=-Wall -Werror -g3

.PHONY: all
all: main

main: $(OBJECTS)
	$(info doing [$@])
	$(Q)gcc -o $@ $(OBJECTS)

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)rm -f $(OBJECTS)
