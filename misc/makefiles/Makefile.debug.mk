# This makefile shows how to do a debug construct in GNU makefiles.

# What is a debug construct ?
# It's a boolean flag that you can set on or off and which controls the emittion of
# various informative messages all around in your makefile...

# Here is out variable (set to 0 or 1)
DO_DEBUG=0
DO_DEBUG=0

ifeq ($(DO_DEBUG),1)
do_debug=@echo $(1)
do_cmd=$(1)
else # DO_DEBUG
do_debug=
do_cmd=@echo "doing $(3) (rule $(2)))"; \@$(1)
endif # DO_DEBUG

.PHONY: all
all:
	$(call do_debug,hello world)
	@echo This is a phony target

# this is even better...
bar: foo
	$(call do_cmd,touch bar,making bar from foo,$@)
