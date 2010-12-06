# this makefile demostrates how to always run a shell command
# (with no regard to targets being built)
# you can check the changing date of the tmp.stamp file using
# the stat(1) cmd line tool. 

FOOBAR:=$(shell touch tmp.stamp)

.PHONY: all
all:
	$(info hello)
