# This shows how to search if a program exists inside a Makefile 
# shamelessly stolen from the make info pages...

pathsearch = $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))

.PHONY: all
all:
	@echo $(call pathsearch,ls)
	@echo $(call pathsearch,wx-config)
	@echo $(call pathsearch,wx-con)
