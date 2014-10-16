SOURCES:=$(shell find . -name "*.c" | tr " " "\\ ")
SOURCES:=$(shell cat list.txt)
OBJECTS:=$(addsuffix .o,$(basename $(SOURCES)))

PROG:=main

.PHONY: all
all: $(PROG)

$(PROG): $(OBJECTS)
	gcc -o $@ $<

.PHONY: clean
clean:
	-rm -f $(OBJECTS) $(PROG)

.PHONY: debug
debug:
	$(info SOURCES is $(SOURCES))
	$(info OBJECTS is $(OBJECTS))
