# this is a minimal makefile for demo purposes, keep it that way...
obj-m:=hello.o
hello-objs:=main.o
V:=0
KDIR:=/lib/modules/$(shell uname -r)/build
# in an embedded system:
#KDIR:=[where the system maker gave me kernel header]/build
#ARCH:=arm
#CROSS_COMPILE:=[path to where the tool chain is]/prefix
.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) modules
.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
