# This shows how to write big functions

define is_even
	ifeq ($(1),1)
		VAL=0
	endif
	ifeq ($(1),2)
		VAL=1
	endif
	ifeq ($(1),3)
		VAL=0
	endif
	ifeq ($(1),4)
		VAL=1
	endif
endef

$(eval $(call is_even,3))
IS_EVEN=$(VAL)

.PHONY: all
all:
	$(info IS_EVEN is $(IS_EVEN))
