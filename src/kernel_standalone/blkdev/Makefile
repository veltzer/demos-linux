# this is a minimal makefile for demo purposes, keep it that way...
obj-m:=blkdev.o
blkdev-objs:=main.o
V?=0
# You could also use W=2 and W=3 for more warnings but most kernel headers
# fail those...
W?=1
KDIR?=/lib/modules/$(shell uname -r)/build
# in an embedded system:
#KDIR:=[where the system maker gave me kernel headers+makefile or the kernel itself which has a makefile]
#ARCH:=arm
#CROSS_COMPILE:=[path+common prefix to where the toolchain is]
.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) W=$(W) modules
.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) V=$(V) clean
