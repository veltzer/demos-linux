# This makefile shows how to know if a variable is defined or not
#
# References:
# http://stackoverflow.com/questions/20389064/using-gnu-make-functions-to-check-if-variables-are-defined

check-var-defined = $(if $(strip $($1)),,$(error "$1" is not defined))
is-var-defined-simple = $(if $(strip $($1)),1,0)
is-var-defined = $(if $(findstring $1, $(.VARIABLES)),1,0)

DEFINED_AND_NOT_EMPTY:=5
$(call check-var-defined,DEFINED_AND_NOT_EMPTY)

DEFINED_AND_EMPTY:=
# this will cause an error, but is actually not right
#$(call check-var-defined,DEFINED_AND_EMPTY)

ifeq ($(call is-var-defined,DEFINED_AND_EMPTY),1)
	GOT_THERE=1
endif

.PHONY: all
all:
	$(info GOT_THERE is $(GOT_THERE))
	$(info is-var-defined-simple(DEFINED_AND_NOT_EMPTY) is $(call is-var-defined-simple,DEFINED_AND_NOT_EMPTY))
	$(info is-var-defined-simple(DEFINED_AND_EMPTY) is $(call is-var-defined-simple,DEFINED_AND_EMPTY))
	$(info is-var-defined-simple(NOT_DEFINED) is $(call is-var-defined-simple,NOT_DEFINED))
	$(info is-var-defined(DEFINED_AND_NOT_EMPTY) is $(call is-var-defined,DEFINED_AND_NOT_EMPTY))
	$(info is-var-defined(DEFINED_AND_EMPTY) is $(call is-var-defined,DEFINED_AND_EMPTY))
	$(info is-var-defined(NOT_DEFINED) is $(call is-var-defined,NOT_DEFINED))
