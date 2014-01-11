# This makefile shows how to show all variables

.PHONY: all
all:
	$(info .VARIABLES is $(.VARIABLES))

.PHONY: each
each:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
