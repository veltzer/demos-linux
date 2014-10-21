# This example explores what variables you can and cannot override from
# the command line of make
# To test this just run this makefile like this:
# make -f Makefile.command_line_override.mk VAR_QUESTION=new_value
# make -f Makefile.command_line_override.mk VAR_COLON=new_value
# make -f Makefile.command_line_override.mk VAR_EQUALS=new_value
# they all work!
# now lets try with environment variables:
# export VAR_QUESTION=new_value
# export VAR_COLON=new_value
# export VAR_EQUALS=new_value
# make -f Makefile.command_line_override.mk
# in this case only the variable with the question mark got the value.
# so the question mark is only relevant to environment variables
# while variables on the make command line always have effect.

VAR_QUESTION?=var_question_orig_value
VAR_COLON:=var_colon
VAR_EQUALS=var_equals

.PHONY: all
all:
	$(info VAR_QUESTION is $(VAR_QUESTION))
	$(info VAR_COLON is $(VAR_COLON))
	$(info VAR_EQUALS is $(VAR_EQUALS))
