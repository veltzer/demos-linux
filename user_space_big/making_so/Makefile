SO_LINK_NAME:=add
SO_NAME:=lib$(SO_LINK_NAME).so
EXE_NAME:=main.exe
SO_OBJECTS:=add.o
EXE_OBJECTS:=main.o
CFLAGS:=-O2 -Wall -Werror

# on Intel this usually works without the -fpic but use it none
# the less, you can also add -nostdlib to avoid the default 
# bahviour of linking with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB=-shared -fpic
# These are the libraries used by your library
EXTRA_LIBS=-lm

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_NAME)
	$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_LINK_NAME)
$(SO_NAME): $(SO_OBJECTS) 
	$(CC) $(FLAGS_FOR_SHARED_LIB) -o $(SO_NAME) $(SO_OBJECTS) $(EXTRA_LIBS)

.PHONY: clean
clean:
	-rm *.o $(EXE_NAME) $(SO_NAME)

.PHONY: run
run:
	LD_LIBRARY_PATH=. ./$(EXE_NAME)

show_results:
	ldd $(SO_NAME) 
	ldd $(EXE_NAME)

show_results_verbose:
	ldd -v $(SO_NAME)
	ldd -u $(SO_NAME)

