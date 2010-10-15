# fill in the object files which are part of the module
obj-m=top.o
# fill in any extra compiler flags
EXTRA_CFLAGS+=-Werror -I.
# fill in the name of the module
name=top
# fill in the name of the genrated ko file
ko-m=top.ko
# fill in the version of the kernel for which you want the module compiled to
KVER?=$(shell uname -r)
# fill in the directory of the kernel build system
KDIR=/lib/modules/$(KVER)/build
# fill in the vervosity level you want
V?=0

CC_SOURCES=$(shell find . -name "*.cc")
CC_OBJECTS=$(addsuffix .o,$(basename $(CC_SOURCES)))

# this rule was also taken from running with V=1
$(ko-m): top.o top.mod.o $(CC_OBJECTS) 
	ld -r -m elf_i386 --build-id -o $(ko-m) top.o top.mod.o $(CC_OBJECTS)
# how was this monstrosity created?
# I ran the build with V=1 and registered the command to compile via gcc.
# picked the same version g++ and gave it the entire flag set (especially the -f stuff).
# removed all -D, preprocessor and
# -ffreestanding -Wno-pointer-sign -Wdeclaration-after-statement
# -Werror-implicit-function-declaration -Wstrict-prototypes
# which are not relevant to C++ (the compiler told me so!)
%.o: %.cc
	 g++-4.4 -nostdinc -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -fno-stack-protector -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -fomit-frame-pointer -Werror -c -o $@ $<
top.o top.mod.o: top.c
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) modules
	-rm -f top.ko
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) modules
modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) modules_install
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) clean
	# some how this file does get cleaned
	-rm -f modules.order
help:
	$(MAKE) -C $(KDIR) M=$(PWD) V=$(V) help
insmod:
	sudo insmod $(ko-m) 
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
