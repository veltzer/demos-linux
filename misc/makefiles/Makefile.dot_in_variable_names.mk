# This example explores limitations on variable name in make(1)
# results:
# - variables can be small case
# - variables can have dots in them
# - variables seem to be quite long

A.B:=this is the value of A.B
a.b:=this is the value of a.b
this.is.a.very.long.name.for.a.variable.indeed.lets.see.if.it.works:=value

.PHONY: all
all:
	$(info A.B is $(A.B))
	$(info a.b is $(a.b))
	$(info this.is.a.very.long.name.for.a.variable.indeed.lets.see.if.it.works is $(this.is.a.very.long.name.for.a.variable.indeed.lets.see.if.it.works))
