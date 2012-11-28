CXXFLAGS:=-I../../include -I.

.PHONY: all
all: bar.o zoo.o ins.o

bar.o: bar.cc
	$(Q)g++ $(CXXFLAGS) -Wall -Werror -c -o bar.o bar.cc
zoo.o: zoo.cc
	$(Q)g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o zoo.o zoo.cc
ins.o: ins.cc
	$(Q)g++ $(CXXFLAGS) -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o ins.o ins.cc

.PHONY: nm
nm:
	$(Q)nm -C bar.o
	$(Q)nm -C zoo.o
	$(Q)nm -C ins.o

.PHONY: clean
clean:
	$(Q)-rm -f *.o
