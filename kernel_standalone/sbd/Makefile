# fill in the object files which are part of the module
obj-m=sbd.o
# fill in the name of the module
#name=sbd
# fill in the name of the genrated ko file
ko-m=sbd.ko
# fill in the version of the kernel for which you want the module compiled to
KVER?=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
V?=0

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) clean
	# some how this file does get cleaned
	-rm -f modules.order
help:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) help
