# fill in the object files which are part of the module
obj-m=params.o
# fill in any extra compiler flags
EXTRA_CFLAGS+=-Werror
# fill in the name of the module
name=params
# fill in the name of the genrated ko file
ko-m=params.ko
# fill in the version of the kernel for which you want the module compiled to
KVER=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR=/lib/modules/$(KVER)/build

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	# some how this file does get cleaned
	-rm -f modules.order Module.symvers
help:
	$(MAKE) -C $(KDIR) M=$(PWD) help
insmod:
	sudo insmod ./$(ko-m)
lsmod:
	sudo lsmod | grep $(name)
rmmod:
	sudo rmmod $(name)
last:
	sudo tail /var/log/kern.log
log:
	sudo tail -f /var/log/kern.log
halt:
	sudo halt
reboot:
	sudo reboot
tips:
	@echo "do make V=1 [target] to see more of what is going on"
	@echo
	@echo "in order for the operational targets to work you need to"
	@echo "make sure that can do 'sudo', preferably with no password."
	@echo "one way to do that is to add yourself to the 'sudo' group"
	@echo "and add to the /etc/sudoers file, using visudo, the line:"
	@echo "%sudo ALL=NOPASSWD: ALL"
	@echo
	@echo "you can compile your module to a different kernel version"
	@echo "like this: make KVER=2.6.13 [target]"
	@echo "or edit the file and permanently change the version"
