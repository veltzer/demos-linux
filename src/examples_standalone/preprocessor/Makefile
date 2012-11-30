# parameters for the makefile
ALL:=defs_c.out defs_cc.out
MAKEFILE:=Makefile

# here we go...
.PHONY: all
all: $(ALL) 

defs_c.out: $(MAKEFILE)
	$(Q)echo | gcc -E -dM - > $@
defs_cc.out: $(MAKEFILE) 
	$(Q)echo | g++ -E -dM - > $@

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL)

.PHONY: debug
debug:
	$(info MAKEFILE is $(MAKEFILE))
