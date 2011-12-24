DIRS=$(shell find . -mindepth 1 -maxdepth 1 -type d)

.PHONY: all clean echo

all:
	for x in $(DIRS); do cd $$x; make; cd ..; done
clean:
	for x in $(DIRS); do cd $$x; make clean; cd ..; done
