SO_LINK_NAME:=add
SO_NAME:=lib$(SO_LINK_NAME).so
EXE_NAME:=main.exe
SO_OBJECTS:=add.o
EXE_OBJECTS:=main.o
CFLAGS:=-O2 -g3 -Wall -Werror
LDFLAGS:=-Wall -Werror
CC:=gcc
ALL_DEPS:=Makefile

# on Intel this usually works without the -fpic but use it none
# the less, you can also add -nostdlib to avoid the default 
# bahviour of linking with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared
# These are the libraries used by your library
LIBS:=

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_NAME) $(ALL_DEPS)
	$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_LINK_NAME)
$(SO_NAME): $(SO_OBJECTS)  $(ALL_DEPS)
	$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_NAME) $(SO_OBJECTS) $(LIBS)

.PHONY: clean
clean: $(ALL_DEPS)
	-rm *.o $(EXE_NAME) $(SO_NAME)

.PHONY: run
run: $(ALL_DEPS)
	LD_LIBRARY_PATH=. ./$(EXE_NAME)

show_results: $(ALL_DEPS)
	ldd $(SO_NAME) 
	ldd $(EXE_NAME)

show_results_verbose: $(ALL_DEPS)
	ldd -v $(SO_NAME)
	ldd -u $(SO_NAME)
