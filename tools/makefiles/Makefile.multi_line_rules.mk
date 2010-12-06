# The example here shows how to define multi line rules...
#
# Notice that you may use multi line rules without the ; or the backslash
# but each of these will be executed in it's own shell context.
# what you really want is this example if you want to pass varialbes etc.

.PHONY: all
all:
	AA=`ls -l /etc | wc -l`; \
	echo $$AA
