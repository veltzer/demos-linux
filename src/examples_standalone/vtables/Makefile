SOURCES=$(shell find . -name "*.cc")
OBJECTS=$(addsuffix .o,$(basename $(SOURCES)))

.PHONY: all
all: main.exe

main.exe: $(OBJECTS)
	$(Q)$(CXX) -o $@ $^
%.o: %.cc
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f $(OBJECTS) main.exe

.PHONY: debug
debug:
	$(info SOURCES is $(SOURCES))
	$(info OBJECTS is $(OBJECTS))

.PHONY: show
show:
	$(Q)for x in *.o; do echo $$x; nm -C $$x | grep vtable ; done
