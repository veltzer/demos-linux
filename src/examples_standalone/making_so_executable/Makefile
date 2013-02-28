include ../../../Makefile.defs
SO_LINK_NAME:=add
SO_NAME:=lib$(SO_LINK_NAME).so
EXE_NAME:=main.$(SUFFIX_BIN)
SO_OBJECTS:=add.o
EXE_OBJECTS:=main.o
CFLAGS:=-O2 -g3 -Wall -Werror -I../../include
LDFLAGS:=-Wall -Werror
CC:=gcc
ALL_DEPS:=Makefile

# on Intel this usually works without the -fpic but use it none
# the less, you can also add -nostdlib to avoid the default 
# bahviour of linking with standard C/C++ runtime libraries.
# this does not work
FLAGS_FOR_SHARED_LIB:=-shared -Xlinker -e -Xlinker mymain
#FLAGS_FOR_SHARED_LIB:=-shared
# These are the libraries used by your library
LIBS:=

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_NAME) $(ALL_DEPS)
	$(Q)$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_LINK_NAME) -Xlinker -rpath=$(CURDIR)
$(SO_NAME): $(SO_OBJECTS) $(ALL_DEPS)
	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_NAME) $(SO_OBJECTS) $(LIBS)
main.o: main.c
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<
add.o: add.c
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean: $(ALL_DEPS)
	$(Q)-rm -f $(SO_OBJECTS) $(EXE_OBJECTS) $(EXE_NAME) $(SO_NAME)

.PHONY: run
run: $(ALL_DEPS)
	$(Q)LD_LIBRARY_PATH=. ./$(EXE_NAME)

.PHONY: show_results
show_results: all $(ALL_DEPS)
	$(Q)ldd $(SO_NAME) 
	$(Q)ldd $(EXE_NAME)

.PHONY: show_results_verbose
show_results_verbose: all $(ALL_DEPS)
	$(Q)ldd -v $(SO_NAME)
	$(Q)ldd -u $(SO_NAME)
