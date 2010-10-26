SO_LINK_NAME:=add
SO_NAME:=lib$(SO_LINK_NAME).so
EXE_NAME:=main.exe
SO_OBJECTS:=add.o
EXE_OBJECTS:=main.o
CFLAGS:=-O2 -Wall -Werror

.PHONY: all
all: $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJECTS) $(SO_NAME)
	$(CC) -o $(EXE_NAME) $(EXE_OBJECTS) -L. -l$(SO_LINK_NAME)
$(SO_NAME): $(SO_OBJECTS) 
	$(CC) -shared -fpic -o $(SO_NAME) $(SO_OBJECTS)

.PHONY: clean
clean:
	-rm *.o $(EXE_NAME) $(SO_NAME)

.PHONY: run
run:
	LD_LIBRARY_PATH=. ./$(EXE_NAME)
