CXXFLAGS:=-I../../include -Wall -Werror -fno-implicit-inline-templates -fno-implicit-templates
OBJECTS:=bar.o zoo.o ins.o

.phony: all
all: out.$(SUFFIX_BIN)

out.$(SUFFIX_BIN): $(OBJECTS)
	$(Q)g++ $(OBJECTS) -o $@

bar.o: bar.cc
	$(Q)g++ $(CXXFLAGS) -c -o bar.o bar.cc
zoo.o: zoo.cc
	$(Q)g++ $(CXXFLAGS) -c -o zoo.o zoo.cc
ins.o: ins.cc
	$(Q)g++ $(CXXFLAGS) -c -o ins.o ins.cc

.PHONY: nm
nm:
	$(Q)nm -C bar.o
	$(Q)nm -C zoo.o
	$(Q)nm -C ins.o

.PHONY: clean
clean:
	$(Q)-rm -f $(OBJECTS) out.$(SUFFIX_BIN)
