# fill in the object files which are part of the module
obj-m=hello.o
# fill in the name of the module
#name=hello
# fill in the name of the genrated ko file
ko-m=hello.ko
# fill in the version of the kernel for which you want the module compiled to
KVER?=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
V?=1

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) clean
	# some how this file does get cleaned
	-rm -f modules.order
help:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) help
