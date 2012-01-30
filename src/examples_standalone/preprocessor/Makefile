# parameters for the makefile
ALL:=defs_c.out defs_cc.out

# here we go...
.PHONY: all
all: $(ALL) 

defs_c.out: $(MAKEFILE)
	echo | gcc -E -dM - > $@
defs_cc.out: $(MAKEFILE) 
	echo | g++ -E -dM - > $@

.PHONY: clean
clean:
	rm -f $(ALL)

.PHONY: debug
debug:
	$(info MAKEFILE is $(MAKEFILE))
