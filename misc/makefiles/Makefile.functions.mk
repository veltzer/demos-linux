# this example demos how to use functions in a makefile...

reverse = $(2) $(1)

all:
	@echo $(call reverse,world,hello)
