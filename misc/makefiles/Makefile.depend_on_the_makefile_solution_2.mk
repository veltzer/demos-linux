# this is a second solution to make sure all targets are recreated whenever
# the makefile changes. In this solution you just have to have a 'clean'
# like target. The advantage of this solution over the one which requires
# a variable to be added to *every rule* is just that. This one is localized.
#
# This will work for most targets, that is targets that are actual files
# and that are removed by clean, and any targets that depend on them.
# Side-effect targets and some PHONY targets will slip through the net.
#
# References:
# http://stackoverflow.com/questions/3871444/making-all-rules-depend-on-the-makefile-itself

MY_MAKEFILE_NAME:=$(lastword $(MAKEFILE_LIST))

ALL:=/tmp/copy_of_passwd /tmp/copy_of_hosts

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -f $(ALL)

/tmp/copy_of_passwd: /etc/passwd
	cp $< $@

/tmp/copy_of_hosts: /etc/hosts
	cp $< $@

# the solution starts here

-include /tmp/dummy

/tmp/dummy: $(MY_MAKEFILE_NAME)
	$(MAKE) --silent -f $(MY_MAKEFILE_NAME) clean
	touch $@
