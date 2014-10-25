# this is one solution to make sure all targets are recreated whenever
# the makefile changes. In this solution you must add the variable
# (ALL_DEPS in this case) to *every rule in your makefile*
# now if you touch this makefile all targets will get recreated.

ALL_DEPS:=$(lastword $(MAKEFILE_LIST))

.PHONY: all
all: /tmp/copy_of_passwd /tmp/copy_of_hosts

/tmp/copy_of_passwd: /etc/passwd $(ALL_DEPS)
	cp $< $@

/tmp/copy_of_hosts: /etc/hosts $(ALL_DEPS)
	cp $< $@
