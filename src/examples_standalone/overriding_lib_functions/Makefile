include ../../../Makefile.defs

SO_LINK_NAME:=mymath
SO_NAME:=lib$(SO_LINK_NAME).so
EXE_NAME:=main.$(SUFFIX_BIN)
SO_OBJECTS:=cos.o
EXE_OBJECTS:=main.o
# on Intel this usually works without the -fpic but use it none the less
CFLAGS:=-O2 -fpic -Wall -Werror -I../../include
LDFLAGS:=-Wall -Werror
CC:=gcc
ALL_DEPS:=Makefile

# you can also add -nostdlib to avoid the default ahviour of linking
# with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared
# These are the libraries used by your library
#LIBS:=-lm
LIBS:=
#MATHLIB:=/libx32/libm-2.17.so
MATHLIB:=

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_NAME) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_LINK_NAME) -Xlinker -rpath=$(CURDIR)
$(SO_NAME): $(SO_OBJECTS) $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(CC) $(LDFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $(SO_NAME) $(SO_OBJECTS) $(MATHLIB) $(LIBS)

%.o: %.c
	$(info doing [$@])
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)-rm -f *.o $(EXE_NAME) $(SO_NAME)
