CFLAGS=-I.
SOURCES=$(shell find . -name "*.c")
BASENAMES=$(basename $(SOURCES))
DEPS=$(addsuffix .d,$(BASENAMES))
OBJECTS=$(addsuffix .o,$(BASENAMES))

ADD_SOURCES=$(shell find libadd -name "*.c")
ADD_BASENAMES=$(basename $(ADD_SOURCES))
ADD_OBJECTS=$(addsuffix .o,$(ADD_BASENAMES))

EXE_SOURCES=$(shell find binmain -name "*.c")
EXE_BASENAMES=$(basename $(EXE_SOURCES))
EXE_OBJECTS=$(addsuffix .o,$(EXE_BASENAMES))

all: $(OBJECTS) $(DEPS) libadd.so main.exe

libadd.so: $(ADD_OBJECTS)
	$(CC) -shared -o libadd.so $(ADD_OBJECTS)

main.exe: $(EXE_OBJECTS) libadd.so
	$(CC) -o main.exe $(EXE_OBJECTS) -L. -ladd

clean:
	-rm $(OBJECTS) $(DEPS) libadd.so main.exe

debug:
	echo $(DEPS)

# implicit rule to generate the deps files using the compiler
# MM and not M is used to only list project file information
%.d: %.c
	$(CC) -E -MM $(CFLAGS) -MT $(addsuffix .o,$(basename $@)) -o $@ $<
	
# include the deps files (no warnings)
-include $(DEPS)
