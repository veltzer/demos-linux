# This example shows how to know the name of the makefile you are in
# if you activate this makefile using "make -C" then CURDIR will
# be the folder that you told make(1) to cd into...

MYMAKE=$(lastword $(MAKEFILE_LIST))

.PHONY: all
all:
	$(info MAKEFILE_LIST=$(MAKEFILE_LIST))
	$(info MAKEFILES=$(MAKEFILES))
	$(info CURDIR=$(CURDIR))
	$(info MYMAKE=$(MYMAKE))
