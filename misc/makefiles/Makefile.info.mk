# This makefile demostrates the $(info XXXX) function
# The first $(info) is just like echo (only cheeper on runtime and can be used
#	as a function which is better)
# The second $(info) is just a newline. Note that you must keep the space.
# The third $(info) shows you that you cannot redirect the output of $(info) to a file.
#	Just the screen. Use the shells echo for that.

.PHONY: all
all:
	$(info This is a message to the screen)
	$(info )
	$(info This is a message to a file) > foo
