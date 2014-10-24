# This makefile explores the A=B for as opposed to the A:=B form for variables...
# the results:
# - using ':=' defines the variable NOW which means that if the variables you depend
# upon are not yet defined you get an empty value
# - using '=' remembers the definition and interprets it when the variable is needed.
# - doing += on the variable does not change its type which is determined on whether
# you used ':=' or '=' to define it. 

A=$(CONSTANT_EQ)
B:=$(CONSTANT_EQ)
C=$(CONSTANT_EQ)
C+=added
D:=$(CONSTANT_EQ)
D+=added

E=$(CONSTANT_CL)
F:=$(CONSTANT_CL)
G=$(CONSTANT_CL)
G+=added
H:=$(CONSTANT_CL)
H+=added

CONSTANT_EQ=hello
CONSTANT_CL:=hello

all:
	@echo A is $(A)
	@echo B is $(B)
	@echo C is $(C)
	@echo D is $(D)
	@echo E is $(E)
	@echo F is $(F)
	@echo G is $(G)
	@echo H is $(H)
