include ../../../Makefile.mk

SO_ADD_LINK_NAME:=add
SO_ADD_NAME:=lib$(SO_ADD_LINK_NAME).so
SO_ADD_OBJECTS:=add.o
SO_SUB_LINK_NAME:=sub
SO_SUB_NAME:=lib$(SO_SUB_LINK_NAME).so
SO_SUB_OBJECTS:=sub.o
SO_MIS_LINK_NAME:=mis
SO_MIS_NAME:=lib$(SO_MIS_LINK_NAME).so
SO_MIS_OBJECTS:=mis.o
EXE_NAME:=main.$(SUFFIX_BIN)
EXE_OBJECTS:=main.o
# on Intel this usually works without the -fpic but use it none the less
CFLAGS:=-O2 -fpic -Wall -Werror -I../../include -I.
LDFLAGS:=
CC:=gcc
ALL_DEPS:=Makefile

# you can also add -nostdlib to avoid the default ahviour of linking
# with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared
# These are the libraries used by your library
LIBS:=

.PHONY: all
all:

.PHONY: fake_all
fake_all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_ADD_NAME) $(SO_SUB_NAME) $(SO_MIS_NAME) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) -o $(EXE_NAME) $(EXE_OBJECTS)
#	$(Q)$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_ADD_LINK_NAME) -Xlinker -rpath=$(CURDIR)
#	$(Q)$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_ADD_LINK_NAME) -Xlinker -rpath=$(CURDIR) -Xlinker --no-undefined
$(SO_ADD_NAME): $(SO_ADD_OBJECTS) $(ALL_DEPS) $(SO_SUB_NAME)
	$(info doing [$@])
	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_ADD_NAME) $(SO_ADD_OBJECTS) $(LIBS)
#	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_ADD_NAME) $(SO_ADD_OBJECTS) $(LIBS) -Xlinker -rpath=$(CURDIR) -L. -lsub
$(SO_SUB_NAME): $(SO_SUB_OBJECTS) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_SUB_NAME) $(SO_SUB_OBJECTS) $(LIBS)
$(SO_MIS_NAME): $(SO_MIS_OBJECTS) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_MIS_NAME) $(SO_MIS_OBJECTS) $(LIBS)
#$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_MIS_NAME) $(SO_MIS_OBJECTS) $(LIBS) -Xlinker --no-undefined

%.o: %.c
	$(info doing [$@])
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)-rm -f *.o $(EXE_NAME) *.so *.stamp

.PHONY: run
run: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)LD_LIBRARY_PATH=. ./$(EXE_NAME)

show_results: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)ldd $(SO_NAME)
	$(Q)ldd $(EXE_NAME)

show_results_verbose: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)ldd -v $(SO_NAME)
	$(Q)ldd -u $(SO_NAME)
