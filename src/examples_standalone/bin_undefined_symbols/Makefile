include ../../../Makefile.mk

EXE_NAME:=main.$(SUFFIX_BIN)
EXE_OBJECTS:=main.o
# on Intel this usually works without the -fpic but use it none the less
CFLAGS:=-O2 -fpic -Wall -Werror -I../../include -I.
LDFLAGS:=-Xlinker --noinhibit-exec -Xlinker --unresolved-symbols=ignore-all
CC:=gcc
ALL_DEPS:=Makefile

# you can also add -nostdlib to avoid the default ahviour of linking
# with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared
# These are the libraries used by your library
LIBS:=

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CXX) $(LDFLAGS) -o $(EXE_NAME) $(EXE_OBJECTS)

%.o: %.cc
	$(info doing [$@])
	$(Q)$(CXX) $(CXXFLAGS) -c -o $@ $<

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

.PHONY: debug
debug:
	$(info EXE_NAME is $(EXE_NAME))
