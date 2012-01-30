# this is a minimal makefile for demo purposes, keep it that way...
obj-m:=hello.o
hello-objs:=main.o
KDIR:=/lib/modules/$(shell uname -r)/build
.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules
.PHONY: clean
clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
