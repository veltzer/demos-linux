# this examples shows how to set the default goal in GNU make
# if it is not set then the first target will be the default goal
# you don't have to put the .DEFAULT_GOAL at the beinging of the
# makefile. Maybe a more logical place would be right before
# the target which is really the default goal...

.DEFAULT_GOAL=all

.PHONY: echo
echo:
	echo $(.DEFAULT_GOAL)

.PHONY: dummy
dummy:
	@echo dummy

.PHONY: all
all:
	@echo all
