# This makefile shows that writing rules of the form:
# a b: c
# 	[cmd]
# will run the rule for a and b once.
# this is what you want if the command in question produces
# both a and b.

SOURCE=source.junk
TG1=file1.junk
TG2=file2.junk

.PHONY: all
all: $(TG1) $(TG2)

$(TG1) $(TG2): $(SOURCE)
	echo $@
	touch $(TG1) $(TG2)

.PHONY: clean
clean:
	-rm -f *.junk
