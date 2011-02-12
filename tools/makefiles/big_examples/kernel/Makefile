# Makefile for hello world kernel 2.6 module.              -*-makefile-*-
#
# Copyright (C) 2004 Joachim Nilsson <joachim.nilsson@member.fsf.org>
#
# Licensed under the GNU General Public License, v2.0 or later,
# at your option.  See the file COPYING for details, or the web
# page http://www.gnu.org/copyleft/gpl.html
#


MODULE_NAME = param
$(MODULE_NAME)-objs = main.o

RM      = @rm -rf
JUNK	= *~ *.bak DEADJOE


# First pass, kernel Makefile reads module objects
ifneq ($(KERNELRELEASE),)
obj-m	:= $(MODULE_NAME).o

EXTRA_CFLAGS += -I$(PWD)/../include

# Second pass, the actual build.
else
KVER    ?= $(shell uname -r)
KDIR	:= /lib/modules/$(KVER)/build
PWD	:= $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

distclean: clean
	$(RM) $(JUNK) $(OBJS)

help:
	$(MAKE) -C $(KDIR) M=$(PWD) help


# Indents the kernel source the way linux/Documentation/CodingStyle.txt
# wants it to be.
indent:
	indent -kr -i8 $($(MODULE_NAME)-objs:.o=.c)

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

endif
