# This makefile explores the A=B for as opposed to the A:=B form for variables...

A=$(C)
B:=$(C)
B+=$(C)
C=hello

all:
	@echo A is $(A)
	@echo B is $(B)
