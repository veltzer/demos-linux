# This makefile shows that writing rules of the form:
# a b: c
# 	[cmd]
# could potentialy run the rule once for a and once for b.
# You write the rule using the automatic varialble $@
# and so you know which target you are creating.
#
# The problem with this approach is that the GNU make automatic $@ variable
# does not really refer to the whole set of targets but only to the one
# currently asked to be created.
#
# This means that writing a multi-target rule in make is just a short hand
# for writing two rules which means that make doesn't REALLY support multiple
# targets at a deep level. This also explains why there is no automatic variable
# in GNU make which refers to ALL the targets.
#
# If you create both targets in a 2 target rule then it will be run once since
# after the first run (that creates both targets) GNU make will check the last
# modification times of the relevant files and will see that the second target
# should not be created.

SOURCE=source.file
TG1=file1.junk
TG2=file2.junk
TG3=file3.junk
TG4=file4.junk

.PHONY: all
all: $(TG1) $(TG2) $(TG3) $(TG4)

$(TG1) $(TG2): $(SOURCE)
	$(info doing $@)
	touch $@

$(TG3) $(TG4): $(SOURCE)
	$(info doing $@)
	touch $(TG3) $(TG4)

.PHONY: clean
clean:
	-rm -f *.junk
