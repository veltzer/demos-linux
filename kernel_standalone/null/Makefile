# name of this module
MODNAME:=null
# ko to make
KO:=$(MODNAME).ko
# fill in the name of the module. I know that it's weird but this should be a .o name
# and is probably due to backwards compatibility issues.
obj-m:=$(MODNAME).o
# This is not strictly required for a single object module where the name of the module
# is the name of the single file in it. In my case this is not the case since the module
# is hello.ko while the single file is main.c.
$(MODNAME)-objs:=main.o
# fill in the version of the kernel for which you want the module compiled to
# the ?= means that it will only be set if you have not provided this via the command line.
# You can override using "make KVER=2.6.35".
KVER?=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR:=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
# You can override using "make V=1"
V?=0

.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules
.PHONY: modules_install
modules_install:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules_install
.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
.PHONY: help
help:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) help
# Indents the kernel source the way linux/Documentation/CodingStyle.txt
# # wants it to be.
.PHONY: indent
indent:
	indent -kr -i8 *.c
.PHONY: insmod
insmod:
	-sudo rmmod $(NAME)
	sudo insmod $(KO)
.PHONY: rmmod
rmmod:
	sudo rmmod $(NAME)
# a target to debug my own makefile...
.PHONY: debug
debug:
	$(info MODNAME is $(MODNAME))
	$(info KO is $(KO))
	$(info KVER is $(KVER))
	$(info KDIR is $(KDIR))
	$(info V is $(V))
	$(info CURDIR is $(CURDIR))
	$(info obj-m is $(obj-m))
	$(info $(MODNAME)-objs is $($(MODNAME)-objs))
