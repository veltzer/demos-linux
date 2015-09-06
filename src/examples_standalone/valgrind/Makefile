include ../../../Makefile.mk

OBJECTS:=leak.dbg.oo leak.opt.oo
OBJECTS_DBG:=leak.dbg.oo
OBJECTS_OPT:=leak.opt.oo
CXXFLAGS_DBG:=-Wall -Werror -g3 -I../../include
CXXFLAGS_OPT:=-Wall -Werror -O2 -I../../include
LDFLAGS:=
ALL:=main.dbg.elf main.opt.elf

.PHONY: all
all: $(ALL)

main.dbg.elf: $(OBJECTS_DBG)
	$(info doing [$@])
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJECTS_DBG)

main.opt.elf: $(OBJECTS_OPT)
	$(info doing [$@])
	$(Q)$(CXX) $(LDFLAGS) -o $@ $(OBJECTS_OPT)

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
