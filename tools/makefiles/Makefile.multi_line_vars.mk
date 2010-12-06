# the idea is to show how to define a multi line variable in makefiles...

MULTI=\
	one\
	three
#	two\


all:
	echo $(MULTI)
