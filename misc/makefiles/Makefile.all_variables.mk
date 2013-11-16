# This makefile shows how to use the .SILENT feature of the makefile...
# Notice that if you simply do "SILENT: " with no depenencies then
# all targets will be silent. If you do specify specific targets then
# only those will be silent.

BUILT_IN_VARS:=$(.VARIABLES)

A:=value_of_a
B:=value_of_b

DEFINED_VARS:=$(filter-out $(BUILT_IN_VARS) BUILT_IN_VARS, $(.VARIABLES))

GPP_PARAMS:=
GPP_PARAMS:=$(foreach v, $(DEFINED_VARS), -D$(v)=\"$($(v))\")

.PHONY: all
all:
	@echo $(DEFINED_VARS)
	@echo $(GPP_PARAMS)
