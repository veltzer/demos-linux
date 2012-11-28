# this makefile shows how to issue complicated (loop?!?) shell commands from make
# these commands will involve both shell variables and make variables

ALL:=a b c

.PHONY: all
all:
	for x in $(ALL); do echo $$x; done

.PHONY: debug
debug:
	$(info ALL is $(ALL))
