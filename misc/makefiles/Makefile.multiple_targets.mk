# This is an example of how to do multiple targets in make.
# This requires a clarification: make does have a feature called multipl targets.
# You write:
# a b : c
# 	[rule]
#
# and make will run the rule twice: once to create a and once to create b.
#
# This is not the notion of multiple targets which is addressed by this example!
# The notion here is a need for a feature like this:
#
# a b : c
# 	[rule]
#
# which will run the rule only once and assume that the actual rule produces both the
# 'a' and the 'b' file.
#
# Make does not support such a feature (at least I didn't find one - please correct
# me if I'm wrong). This means that you have to use the trick below:
#

SOURCE=source
TG1=file1
TG2=file2

stamp: source
	touch $(TG1) # creating target 1
	touch $(TG2) # creating target 2
	touch $@ # create the phony stamp file

# these next rules are so you could build targets individually...

$(TG1): stamp
$(TG2): stamp
