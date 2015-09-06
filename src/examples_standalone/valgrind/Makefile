include ../../../Makefile.mk

#SRC_NAME:=leak
SRC_NAME:=tls_pthread_style.cc
SRC_BASE:=$(basename $(SRC_NAME))

OBJECTS_DBG:=$(SRC_BASE).dbg.oo
OBJECTS_OPT:=$(SRC_BASE).opt.oo
OBJECTS:=$(OBJECTS_DBG) $(OBJECTS_OPT)
CXXFLAGS_DBG:=-Wall -Werror -g3 -I../../include
CXXFLAGS_OPT:=-Wall -Werror -O2 -I../../include
LDFLAGS:=
LDLIBS:=-lpthread
ALL:=main.dbg.elf main.opt.elf

.PHONY: all
all: $(ALL)

main.dbg.elf: $(OBJECTS_DBG)
	$(info doing [$@])
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJECTS_DBG) $(LDLIBS)

main.opt.elf: $(OBJECTS_OPT)
	$(info doing [$@])
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJECTS_OPT) $(LDLIBS)

%.dbg.oo: %.cc
	$(info doing [$@])
	$(Q)$(CXX) -c $(CXXFLAGS_DBG) $< -o $@

%.opt.oo: %.cc
	$(info doing [$@])
	$(Q)$(CXX) -c $(CXXFLAGS_OPT) $< -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)rm -f $(OBJECTS) $(ALL)

.PHONY: check_opt
check_opt:
	$(info doing [$@])
	$(Q)valgrind --tool=memcheck ./main.opt.elf

.PHONY: check_dbg
check_dbg:
	$(info doing [$@])
	$(Q)valgrind --tool=memcheck ./main.dbg.elf

.PHONY: checkverbose_opt
checkverbose_opt:
	$(info doing [$@])
	$(Q)valgrind -v --tool=memcheck ./main.opt.elf

.PHONY: checkverbose_dbg
checkverbose_dbg:
	$(info doing [$@])
	$(Q)valgrind -v --tool=memcheck ./main.dbg.elf

.PHONY: checkfull_opt
checkfull_opt:
	$(info doing [$@])
	$(Q)valgrind --leak-check=full --tool=memcheck ./main.opt.elf

.PHONY: checkfull_dbg
checkfull_dbg:
	$(info doing [$@])
	$(Q)valgrind --leak-check=full --tool=memcheck ./main.dbg.elf
