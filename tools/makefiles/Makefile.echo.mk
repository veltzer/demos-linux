# This shows how supress echoing of commands...

DO_DEBUG=1
ifeq ($(DO_DEBUG),1)
do_cmd=$(1)
else # DO_DEBUG
do_cmd=$(info $(2) $(3)) @$(1)
endif # DO_DEBUG

.PHONY: all
all:
	$(call do_cmd,echo hello,this is the description,this is the target)
