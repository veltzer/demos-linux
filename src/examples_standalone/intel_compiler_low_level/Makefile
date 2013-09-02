include ../../../Makefile.defs

# variables
CC:=icc
CFLAGS:=-Wall -Werror -I../../include

# targets
.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.c
	$(info doing [$@])
	$(Q)$(CC) $(CFLAGS) main.c -o main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f main.$(SUFFIX_BIN) *.stamp

.PHONY: dis
dis:
	$(info doing [$@])
	$(Q)objdump main.$(SUFFIX_BIN) --disassemble > main.dis
