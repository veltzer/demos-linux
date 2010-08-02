SOURCES=$(shell find . -name "*.cc")
OBJECTS=$(addsuffix .o,$(basename $(SOURCES)))

.PHONY: all
all: main.exe

main.exe: $(OBJECTS)
	g++ -o $@ $^

.PHONY: clean
clean:
	-rm -f $(OBJECTS) main.exe

.PHONY: debug
debug:
	$(info SOURCES is $(SOURCES))
	$(info OBJECTS is $(OBJECTS))

.PHONY: show
show:
	for x in *.o; do echo $$x; nm -C $$x | grep W ; done
