# This makefile shows how to show all variables
# note that since make incorporates environment variables
# then you will see all environment variables as well.

.PHONY: all
all:
	$(info .VARIABLES is $(.VARIABLES))

.PHONY: each
each:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
