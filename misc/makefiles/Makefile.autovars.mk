# this example shows the automatic variables of make
# we learn that:
# $^ is all the ingredients
# $< is the leftmost ingredient

test: demo_folder/source.file demo_folder/source1.file
	$(info $$@ is $@)
	$(info $$^ is $^)
	$(info $$< is $<)

.PHONY: all
all: test
	$(info doing all)
