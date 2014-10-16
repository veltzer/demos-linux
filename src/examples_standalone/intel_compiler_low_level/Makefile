include ../../../Makefile.mk

# variables
CC:=icc
CFLAGS:=-Wall -Werror -I../../include

ifeq ($(DO_INTEL),1)
	ALL:=main.$(SUFFIX_BIN)
else
	ALL:=
endif

# targets
.PHONY: all
all: $(ALL)

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
