include ../../../Makefile.defs

SOURCES:=$(shell find . -name "*.cc")
OBJECTS:=$(addsuffix .o,$(basename $(SOURCES)))
CXXFLAGS:=-Wall -Werror -I../../include

.PHONY: all
all: main.$(SUFFIX_BIN)

main.$(SUFFIX_BIN): $(OBJECTS)
	$(Q)$(CXX) -o $@ $^
%.o: %.cc
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f $(OBJECTS) main.$(SUFFIX_BIN)

.PHONY: debug
debug:
	$(info SOURCES is $(SOURCES))
	$(info OBJECTS is $(OBJECTS))

.PHONY: show
show:
	$(Q)for x in *.o; do echo $$x; nm -C $$x | grep vtable ; done
