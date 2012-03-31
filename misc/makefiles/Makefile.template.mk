# this makefile shows how to define a piece of code to be executed multiple times

#.SECONDEXPANSION:

NAMES:=A B
FILES_A:=/etc/X11/Xresources/
FILES_B:=/etc/ssh

ALL_OBJS:=
define template
	REP_$(1):=$$(FILES_$(1))
	OBJECTS_$(1):=$$(shell find $$(REP_$(1)) -type f)
	OTHERS_$(1):=$$(addsuffix .foobar,$$(OBJECTS_$(1)))
	ALL_OBJS+=$$(OTHERS_$(1))
endef

$(foreach name,$(NAMES),$(eval $(call template,$(name))))

all:
	@echo "REP_A is $(REP_A)"
	@echo "REP_B is $(REP_B)"
	@echo "OBJECTS_A is $(OBJECTS_A)"
	@echo "OBJECTS_B is $(OBJECTS_B)"
	@echo "OTHERS_A is $(OTHERS_A)"
	@echo "OTHERS_B is $(OTHERS_B)"
	@echo "ALL_OBJS is $(ALL_OBJS)"

debug:
	$(foreach name,$(NAMES),$(info OBJECTS_$(name) is $(OBJECTS_$(name))))
	@echo "ALL_OBJS is $(ALL_OBJS)"
