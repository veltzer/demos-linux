base=pipe
obj-m:=$(base).o
MOD_NAME:=$(base).ko
KERNELDIR:=/lib/modules/$(shell uname -r)/build
.PHONY: modules
modules:
	$(MAKE) -C $(KERNELDIR) M=$$PWD modules
.PHONY: clean
clean:
	$(MAKE) -C $(KERNELDIR) M=$$PWD clean
.PHONY: insmod
insmod:
	sudo insmod $(MOD_NAME)
.PHONY: rmmod
rmmod:
	sudo rmmod $(base)
