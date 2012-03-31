# This makefile knows how to know the name of the current file you are in...

include inc.mk

.PHONY: all
all:
	@echo included name was $(SETUP_NAME)
