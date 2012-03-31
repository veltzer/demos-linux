# This makefile shows how to use the .SILENT feature of the makefile...
# Notice that if you simply do "SILENT: " with no depenencies then
# all targets will be silent. If you do specify specific targets then
# only those will be silent.

DO_ALL_SILENT=1
DO_SILENT=0

ifeq ($(DO_ALL_SILENT),1)
.SILENT:
endif
ifeq ($(DO_SILENT),1)
.SILENT: silent_commands 
endif

.PHONY: silent_commands
silent_commands:
	echo these are silent commands

.PHONY: noisy_commands
noisy_commands:
	echo these are noisy commands
