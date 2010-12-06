# This makefile shows how to use the .DEFAULT target to make sure
# everything gets regenerated when the Makefile changes....

# THIS DOES NOT WORK!!!!

.DEFAULT: Makefile.default.mk

out: in
	cp in out
