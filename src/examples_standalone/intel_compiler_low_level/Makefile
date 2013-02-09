# variables
CC:=icc
CFLAGS:=-Wall -Werror -I../../include

# targets
.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): main.c
	$(Q)$(CC) $(CFLAGS) main.c -o main.$(SUFFIX_BIN)

.PHONY: clean
clean:
	$(Q)-rm -f main.$(SUFFIX_BIN)

.PHONY: dis
dis:
	$(Q)objdump main.$(SUFFIX_BIN) --disassemble > main.dis
