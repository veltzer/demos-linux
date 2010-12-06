# the colon in the next line is very important since the idea is
# to execute the statement as it is being read rather than later.
# (that is the difference between := and =).
# Why? because someone may have already included more files until
# this statement will be executed...
SETUP_NAME:=$(lastword $(MAKEFILE_LIST))
