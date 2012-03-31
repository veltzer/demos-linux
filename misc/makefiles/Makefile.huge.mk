#############################################
# SECTION: on/off switches for the makefile #
#############################################
# should we do dependencies on the Makefile itself or on direct inclusions to the makefile ?
DO_MKDEPS:=1
# should we use rpath when linking dlls and exes
DO_RPATH:=0
# should we use rpath-link when linking dlls and exes
DO_RPATH_LINK:=1
# should we add debug info ?
DO_DEBUG:=0
# should we optimize ?
DO_OPT:=1
# should we do the graphical applications ?
DO_GRA:=1
# should we include dependencies ?
DO_INCLUDE:=1
# should we debug the Makefile itself ?
DO_MKDBG:=0
# should we do static graphical applications ?
DO_STATIC:=0
# should we do the ctags stuff ?
DO_CTAGS:=1
##############################################
# SECTION: INIT - initalizing some variables #
##############################################
# all files that the makefile depends on...
LIST_MKDEPS:=
# all miscelleneous files
ALL_MISC:=
# all miscelleneous files that should be built by default
ALL_MISC_DEFAULT:=
# all object files
ALL_OBJS:=
# all depenency files
ALL_DEPS:=
# all static archives
ALL_ARXS:=
# all binaries
ALL_BINS:=
# all dlls
ALL_LIBS:=
# all kernel modules
ALL_MODS:=
# all headers that we deliver in the package
ALL_HDRS:=
# all headers that we have
ALL_MHDR:=
# folders which should be added to the package
ALL_EFLD:=
# all sources that we have
ALL_SRCS:=
# our sources
ALL_MSRC:=
# all the preprocessed files
ALL_PREP:=
# folders that should be cleaned
ALL_CFLD:=
# files that should be cleaned
ALL_CFIL:=
# docs that should be delivered in the package
ALL_DOCS_DELI:=
# extra ** TARGET NAMES ** that should be activated on clean
ALL_CLEAN_TARGETS:=
#################################################
# SECTION: GENERAL - overall makefile variables #
#################################################
# set the default goal to all so that I could write the makefile with no attention
# to order of targets in it. all is at the end since then all variables are
# well defined...
.DEFAULT_GOAL:=all
# you can change the default goal to not include packing, tests or what ever.
# just remark the above line, unremark the following line and select any subset
# in the following list...
#.DEFAULT_GOAL:=all_misc all_bins all_libs all_arxs all_objs all_deps all_test all_mods
# should we do static executables (not implemented)
LIST_MKDEPS+=Makefile
# lets get the NUMAKE definitions
NUMAKE_FILE:=driver/NUMAKE
LIST_MKDEPS+=$(NUMAKE_FILE)
include $(NUMAKE_FILE)
# include the build configuation file
CONFIG:=config.mk
CONFIG_REAL:=$(shell readlink $(CONFIG))
LIST_MKDEPS+=$(CONFIG)
ifneq ($(MAKECMDGOALS),print_name)
ifneq ($(MAKECMDGOALS),print_version)
include $(CONFIG)
endif # print_version
endif # print_name

# handling makefile dependencies

ifeq ($(DO_MKDEPS),1)
ALL_MKDEPS:=$(LIST_MKDEPS)
else # DO_MKDEPS
ALL_MKDEPS:=
endif # DO_MKDEPS

# some handy functions (the must not use :=. Understand ?!?)

ifeq ($(DO_RPATH),1)
link_to=-Xlinker -rpath -Xlinker $(OBJP)lib -L$(OBJP)lib $(addprefix -l,$(1))
elink_to=$(addprefix -l,$(1))
rpath_to=-Xlinker -rpath -Xlinker $(1)
else # DO_RPATH
link_to=-L$(OBJP)lib $(addprefix -l,$(1))
elink_to=$(addprefix -l,$(1))
rpath_to=
endif # DO_RPATH

ifeq ($(DO_RPATH_LINK),1)
link_to=-Xlinker -rpath-link -Xlinker $(OBJP)lib -L$(OBJP)lib $(addprefix -l,$(1))
elink_to=$(addprefix -l,$(1))
rpath_to=-Xlinker -rpath-link -Xlinker $(1)
else # DO_RPATH_LINK
link_to=-L$(OBJP)lib $(addprefix -l,$(1))
elink_to=$(addprefix -l,$(1))
rpath_to=
endif # DO_RPATH_LINK

# parameters to do_mkdbg are:
# parameters to do_mkdbg are:
# 1 - rule name
# 2 - target of the rule
do_mkdbg=$(info doing [$(2)] ($(1)))
do_smkdbg=$(info doing ($(1)))
ifeq ($(DO_MKDBG),1)
Q=
# we are not silent in this branch
else # DO_MKDBG
Q=@
endif # DO_MKDBG

to_lib=$(addsuffix .so,$(addprefix $(OBJP)lib/lib,$(1)))
to_ar=$(addsuffix .a,$(addprefix $(OBJP)ar/lib,$(1)))
to_modname=$(addsuffix .ko,$(addprefix $(OBJP)modules/,$(1)))
to_objs=$(addprefix $(OBJP)objs/,$(addsuffix .o,$(basename $(1))))
to_deps=$(addprefix $(OBJP)deps/,$(addsuffix .d,$(basename $(1))))
to_bins=$(addprefix $(OBJP)bin/,$(basename $(notdir $(1))))
to_prep=$(addprefix $(OBJP)prep/,$(addsuffix .p,$(basename $(1))))
to_bin=$(addprefix $(OBJP)bin/,$(1))
to_sbin=$(addsuffix .static,$(addprefix $(OBJP)bin/,$(1)))
# function which searches for an executable in the PATH environment variable
pathsearch=$(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))

# end of functions

LINK_PTHREAD:=-lpthread
AR_TOOL:=ar crs
DEPS_FLAGS:=-E -MM -MT
HOSTNAME:=$(shell hostname)
DATE:=$(shell date)
UNAMEA:=$(shell uname -a)
USER:=$(shell id -un)
#LSB:=$(shell lsb_release -a)

# some wx constants

WX_CONFIG:=$(call pathsearch,wx-config)
ifneq ($(WX_CONFIG),)
WX_CPP:=$(shell $(WX_CONFIG) --cxxflags)
WX_LD:=$(shell $(WX_CONFIG) --libs)
# the next line does not work (wx on ubuntu does not support --libs --static)
#WX_LD_STATIC:=-static $(shell $(WX_CONFIG) --libs --static)
WX_LD_STATIC:=-static $(shell $(WX_CONFIG) --libs)
else # WX_CONFIG
DO_GRA:=0
endif # WX_CONFIG

# this is where all targets are stored
OBJP:=obj/
# basic flags for c and linking
# -pedantic or -pedantic-errors seem too strict.
# what about -Wextra
#CFLAGS:=-Wall -Wextra -Werror
CFLAGS:=-Wall -Werror
LDFLAGS:=
KERNEL_KCFLAGS:=-Werror

ifeq ($(DO_DEBUG),1)
CFLAGS+=-g3
else # DO_DEBUG
LDFLAGS+=-s
endif # DO_DEBUG

ifeq ($(DO_OPT),1)
CFLAGS+=-O2
endif # DO_OPT

CXXFLAGS:=$(CFLAGS)

# an main() function file that does nothing to test linking against...
TEST_EMPTY:=misc/test.cc

# tools
MKDIR:=mkdir -p

#####################################################
# SECTION: SCRIPTS - various scripts for the system #
#####################################################
# where are the scripts
SCRIPTS_DIR:=scripts/
# what are the scripts sources
SCRIPTS_SRCS:=$(shell find $(SCRIPTS_DIR) -type f)

# fake targets
.PHONY: scripts_debug
scripts_debug:
	$(info SCRIPTS_DIR is $(SCRIPTS_DIR))
	$(info SCRIPTS_SRCS is $(SCRIPTS_SRCS))
#################################################################
# SECTION: SHARED - header files for both user space and kernel #
#################################################################

SHARED_DIR:=shared/
SHARED_INC:=-I$(SHARED_DIR)
SHARED_HDRS:=$(shell find $(SHARED_DIR) -name "*.h")

# add to all
ALL_HDRS+=$(SHARED_HDRS)
ALL_MHDR+=$(SHARED_HDRS)

##########################################
# SECTION: NU - kernel c++ support files #
##########################################
# this is the directory which will be searched for nufiles to compile
NU_DIR:=nu/
# this is a flags parameter to be used to include nu files
NU_INC_FLAGS:=-I$(NU_DIR)
# this are the nu sources
NU_SRCS:=$(shell find $(NU_DIR) -name "*.cpp")
NU_HDRS:=$(shell find $(NU_DIR) -name "*.h")
# this are the nu objects
NU_OBJS:=$(call to_objs,$(NU_SRCS))
# these are the nu dep files
NU_DEPS:=$(call to_deps,$(NU_SRCS))

# adding to all...
ALL_SRCS+=$(NU_SRCS)
ALL_MSRC+=$(NU_SRCS)
ALL_DEPS+=$(NU_DEPS)
ALL_OBJS+=$(NU_OBJS)

# rules
$(NU_OBJS): $(OBJP)objs/%.o: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,nu_objs,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_CXX) $(KERN_CXX_FLAGS) -c -o $@ $<
$(NU_DEPS): $(OBJP)deps/%.d: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,nu_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_CXX) $(KERN_CXX_FLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $<
# fake targets
.PHONY: nu_debug
nu_debug:
	$(info NU_SRCS is $(NU_SRCS))
	$(info NU_DEPS is $(NU_DEPS))
	$(info NU_OBJS is $(NU_OBJS))
#################################################
# SECTION: MORRIS - user supplied numega driver #
#################################################
# this is the directory which will be searched for source files to compile
MORRIS_DIR:=$(NUMAKE_DRIVERS_DIR)
# this is a list of directories which should be included in header file search
MORRIS_INC_DIRS:=$(MORRIS_DIR) $(NUMAKE_DRIVERS_INCLUDE)
# these are the sources (all the source code for the driver)
MORRIS_SRCS:=$(NUMAKE_DRIVERS_FILES)
# these are flags which are used to compile the windows sources (overcome them)
MORRIS_FLAGS:=-Wno-multichar -Wno-unknown-pragmas
# these are include directives for the driver
MORRIS_INC_FLAGS:=$(addprefix -I,$(MORRIS_INC_DIRS))
# these are the object files generated from the windows sources
MORRIS_OBJS:=$(call to_objs,$(MORRIS_SRCS))
# these are the post processed files generated from the windows sources
MORRIS_PREP:=$(call to_prep,$(MORRIS_SRCS))
# these are the morris dependency files
MORRIS_DEPS:=$(call to_deps,$(MORRIS_SRCS))
# all files supplied by morris
MORRIS_ALL_FILES:=$(shell find -L driver -type f -and \( -name "*.h" -or -name "*.cpp" \) )

# add to all
ALL_SRCS+=$(MORRIS_SRCS)
# these are NOT my files...
#ALL_MSRC+=$(MORRIS_SRCS)
ALL_DEPS+=$(MORRIS_DEPS)
ALL_OBJS+=$(MORRIS_OBJS)
ALL_PREP+=$(MORRIS_PREP)

# rules
$(MORRIS_OBJS): $(OBJP)objs/%.o: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,morris_objs,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_CXX) $(KERN_CXX_FLAGS) -c -o $@ $<
$(MORRIS_DEPS): $(OBJP)deps/%.d: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,morris_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_CXX) $(KERN_CXX_FLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $<
$(MORRIS_PREP): $(OBJP)prep/%.p: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,morris_prep,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_CXX) $(KERN_CXX_FLAGS) -E -o $@ $<

# fake targets
.PHONY: morris_debug morris_fromdos morris_todos morris_find_extra_files
morris_debug:
	$(info MORRIS_SRCS is $(MORRIS_SRCS))
	$(info MORRIS_OBJS is $(MORRIS_OBJS))
	$(info MORRIS_DEPS is $(MORRIS_DEPS))
	$(info MORRIS_PREP is $(MORRIS_PREP))
	$(info MORRIS_ALL_FILES is $(MORRIS_ALL_FILES))
morris_fromdos:
	fromdos -v $(MORRIS_ALL_FILES)
morris_todos:
	todos -v $(MORRIS_ALL_FILES)
# this target finds files which I do not seem to need in order to compile neither the driver
# nor the user space dlls...
morris_find_extra_files:
	@find driver/ -not -name "*.c" -and -not -name "*.h" -and -not -name "*.cpp" -and -type f -and -not -name "*.inf" -and -not -name "NUMAKE"
#####################################################################
# SECTION: LINUX - here we handle the linux kernel module directory #
#####################################################################
# here is the directory where all the action takes place...
LINUX_DIR:=kernel_module/
# fill in the name of the module
LINUX_NAME:=$(NUMAKE_DRIVERS_NAME)
# the ko file which comes out of the process
LINUX_MOD:=$(call to_modname,$(LINUX_NAME))
# the extra source file
LINUX_TOUCH_FILE:=$(LINUX_DIR)$(LINUX_NAME).c
# the mod file created
LINUX_MOD_FILE:=$(LINUX_DIR)$(LINUX_NAME).mod.c
# the stub file created
LINUX_STUB_FILE:=$(LINUX_DIR)stubs.c
# here are the kernel paramters
LINUX_INC:=-I$(LINUX_DIR)
# these are all the kernel sources needed
LINUX_SRCS_ALL:=$(shell find $(LINUX_DIR) -name "*.c")
# make sure that we do not include the main .o file in the link
LINUX_SRCS_2:=$(filter-out $(LINUX_TOUCH_FILE),$(LINUX_SRCS_ALL))
# make sure that we do not include the mod.c file in the link
LINUX_SRCS_1:=$(filter-out $(LINUX_MOD_FILE),$(LINUX_SRCS_2))
# make sure that we do not include the stub .o file in the link
LINUX_SRCS:=$(filter-out $(LINUX_STUB_FILE),$(LINUX_SRCS_1))
# add the generated file to derive the objects list
LINUX_SRCS_FULL:=$(LINUX_SRCS) $(LINUX_MOD_FILE)
# these are the linux objects needed
LINUX_OBJS:=$(addsuffix .o,$(basename $(LINUX_SRCS_FULL)))
# these are the dependencies needed (we use LINUX_SRCS because we dont need the stub)
LINUX_DEPS:=$(call to_deps,$(LINUX_SRCS))
# these are the linux objects bases
LINUX_BOBJ:=$(notdir $(addsuffix .o,$(basename $(LINUX_SRCS_1))))
# define linux makefile
LINUX_MAKEFILE:=$(LINUX_DIR)Makefile
LINUX_MAKEFILE_TMPL:=$(LINUX_DIR)/Makefile.mk
# creating the pci device listing
LINUX_PCI_SNIP:=$(LINUX_DIR)pci.snip
# linux header files
LINUX_HDRS:=$(shell find $(LINUX_DIR) -name "*.h" -type f)

# linux kernel objects do not reside in the usual place since they are generated
# via the kernel build process... (do not use :=)
to_kobjs=$(addsuffix .o,$(basename $(1)))

# add these flags to kernel compilation
KERN_CXX_FLAGS:=$(KERN_CXX_KFLAGS) -Werror $(SHARED_INC) $(MORRIS_INC_FLAGS) $(MORRIS_FLAGS) $(NU_INC_FLAGS) $(LINUX_INC)
# add the objects to the kernel objects
KERN_OBJS:=$(MORRIS_OBJS) $(NU_OBJS) $(LINUX_OBJS)

# add to all
# add kernel h files which should be exported to users...
ALL_HDRS+=$(LINUX_HDRS)
ALL_MHDR+=$(LINUX_HDRS)
ALL_DEPS+=$(LINUX_DEPS)
ALL_MODS+=$(LINUX_MOD)
ALL_MISC+=$(LINUX_PCI_SNIP)
ALL_MISC_DEFAULT+=$(LINUX_PCI_SNIP)

# rules

$(LINUX_OBJS): $(LINUX_SRCS) $(LINUX_MAKEFILE) $(LINUX_PCI_SNIP) $(ALL_MKDEPS)
	$(MAKE) -C $(LINUX_DIR) clean modules MAKEFLAGS=
# TODO: this needs to be changed to template processing
$(LINUX_MAKEFILE): $(LINUX_MAKEFILE_TMPL) $(ALL_MKDEPS)
	$(call do_mkdbg,linux kernel module makefile,$@)
	$(Q)echo "# DO NOT CHANGE THIS FILE. IT IS GENERATED FROM A TEAMPLATE" > $@
	$(Q)sed 's%KERNEL_MODULE_NAME%$(LINUX_NAME)%g;s%CROSS_COMPILE_TO_USE%$(CROSS_COMPILE_TO_USE)%g;s%KDIR_TO_USE%$(KDIR_TO_USE)%g;s%OBJECTS%$(LINUX_BOBJ)%g;s%VAR_DO_CROSS%$(VAR_DO_CROSS)%g;s%KERNEL_KCFLAGS%$(KERNEL_KCFLAGS)%g' $< >> $@
# this was also taken from running the regular kernel module build process with V:=1
$(LINUX_MOD): $(KERN_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,building kernel module,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(KERN_LD) $(KERN_LD_FLAGS) -o $@ $(KERN_OBJS)
$(LINUX_PCI_SNIP): $(NUMAKE_DRIVERS_INF) ./scripts/parse_inf.pl $(ALL_MKDEPS)
	$(call do_mkdbg,parsing windows inf information,$@)
	$(Q)./scripts/parse_inf.pl $< $@
$(LINUX_DEPS): $(OBJP)deps/%.d: %.c $(ALL_MKDEPS) $(LINUX_PCI_SNIP)
	$(call do_mkdbg,linux_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)gcc -nostdinc -isystem /usr/lib/gcc/i486-linux-gnu/4.3.3/include -D__KERNEL__ -I$(ROOT_KDIR_TO_USE)/include/ -include linux/autoconf.h -I$(ROOT_KDIR_TO_USE)/arch/x86/include/ -I$(ROOT_KDIR_TO_USE)/include/asm-x86/mach-default $(DEPS_FLAGS) $(call to_kobjs,$<) -o $@ $<

# fake targets
.PHONY: linux_mod_insmod linux_mod_lsmod linux_mod_rmmod linux_mod_tips linux_mod_debug linux_modinfo linux_mod_clean linux_mod_modules linux_mod_modules_install linux_mod_help linux_mod_mymod linux_debug linux_objs linux_pcisnip linux_mod_chmod linux_mod_find_symbol linux_mod_lsl linux_deps
linux_mod_insmod: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) insmod MAKEFLAGS=
linux_mod_lsmod: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) lsmod MAKEFLAGS=
linux_mod_rmmod: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) rmmod MAKEFLAGS=
linux_mod_tips: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) tips MAKEFLAGS=
linux_mod_debug: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) debug MAKEFLAGS=
linux_modinfo: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) modinfo MAKEFLAGS=
linux_mod_clean: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) clean MAKEFLAGS=
	-$(Q)rm -f $(LINUX_MOD) $(LINUX_MAKEFILE)
linux_mod_modules: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) clean modules MAKEFLAGS=
	-$(Q)rm -f $(LINUX_MAKEFILE)
linux_mod_modules_install: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) modules_install MAKEFLAGS=
linux_mod_help: $(LINUX_MAKEFILE)
	$(MAKE) -C $(LINUX_DIR) help MAKEFLAGS=
linux_mod_mymod: $(LINUX_MOD)
linux_mod_chmod:
	sudo chmod 666 /dev/KFcb*
LINUX_SYMBOL:=ZN13KFcbBusDevice4ReadE4KIrp
LINUX_SYMBOL:=__divdi3
LINUX_SYMBOL:=__bad_func_type
linux_mod_find_symbol:
	./scripts/find_symbol.pl $(LINUX_SYMBOL) $(KERN_OBJS)
linux_mod_lsl:
	ls -l $(KERN_OBJS)
linux_mod_objs: $(LINUX_OBJS)
linux_mod_pcisnip: $(LINUX_PCI_SNIP)
linux_debug:
	$(info LINUX_NAME is $(LINUX_NAME))
	$(info LINUX_MOD is $(LINUX_MOD))
	$(info LINUX_MOD_FILE is $(LINUX_MOD_FILE))
	$(info LINUX_TOUCH_FILE is $(LINUX_TOUCH_FILE))
	$(info LINUX_STUB_FILE is $(LINUX_STUB_FILE))
	$(info LINUX_SRCS_ALL is $(LINUX_SRCS_ALL))
	$(info LINUX_SRCS_2 is $(LINUX_SRCS_2))
	$(info LINUX_SRCS_1 is $(LINUX_SRCS_1))
	$(info LINUX_SRCS is $(LINUX_SRCS))
	$(info LINUX_SRCS_FULL is $(LINUX_SRCS_FULL))
	$(info LINUX_OBJS is $(LINUX_OBJS))
	$(info LINUX_BOBJ is $(LINUX_BOBJ))
	$(info LINUX_PCI_SNIP is $(LINUX_PCI_SNIP))
	$(info KERN_OBJS is $(KERN_OBJS))
	$(info KERN_CXX_FLAGS is $(KERN_CXX_FLAGS))
linux_deps: $(LINUX_DEPS)
ALL_CLEAN_TARGETS+=linux_mod_clean
####################################
# SECTION: UDEV - support for udev #
####################################
UDEV_SRC:=udev/rule.tmpl
UDEV_RULE:=udev/rule
UDEV_INSTALL:=/lib/udev/rules.d/99-$(LINUX_NAME).rules

# add to all
ALL_MISC+=$(UDEV_RULE)
ALL_MISC_DEFAULT+=$(UDEV_RULE)

# rules
$(UDEV_RULE): $(UDEV_SRC) $(ALL_MKDEPS)
	$(call do_mkdbg,generating udev rule,$@)
	$(Q)sed 's%KERNEL_MODULE_NAME%$(LINUX_NAME)%g;s%CROSS_COMPILE_TO_USE%$(CROSS_COMPILE_TO_USE)%g;s%KDIR_TO_USE%$(KDIR_TO_USE)%g;s%VAR_DO_CROSS%$(VAR_DO_CROSS)%g' $< > $@
$(UDEV_INSTALL): $(UDEV_RULE) $(ALL_MKDEPS)
	$(call do_mkdbg,installing udev rule,$@)
	$(Q)sudo cp $(UDEV_RULE) $(UDEV_INSTALL)

# fake targets
.PHONY: udev_install udev_uninstall udev_restart udev_restart2 udev_install_restart udev_debug
udev_install: $(UDEV_INSTALL)
udev_uninstall:
	sudo rm -f $(UDEV_INSTALL)
udev_restart:
	sudo udevadm control --reload-rules
udev_restart2:
	sudo /etc/init.d/udev reload
udev_install_restart: udev_install udev_restart
udev_debug:
	sudo udevadm monitor --environment

######################################################
# SECTION: KDEV - kernel development helpful targets #
######################################################
# fake targets
KDEV_PHONY:=kdev_kern_tail kdev_kern_tailf kdev_syslog_tail kdev_syslog_tailf kdev_dmesg kdev_dmesg_clean kdev_halt kdev_reboot kdev_cache_debug kdev_insmod kdev_lsmod kdev_rmmod
.PHONY: $(KDEV_PHONY)
ifneq ($(filter $(KDEV_PHONY),$(MAKECMDGOALS)),)
DO_INCLUDE:=0
endif
kdev_kern_tail:
	sudo tail /var/log/kern.log
kdev_kern_tailf:
	sudo tail -f /var/log/kern.log
kdev_syslog_tail:
	sudo tail /var/log/kern.log
kdev_syslog_tailf:
	sudo tail -f /var/log/kern.log
kdev_dmesg:
	sudo dmesg | c++filt
kdev_dmesg_clean:
	sudo dmesg -c > /dev/null
kdev_halt:
	sudo halt
kdev_reboot:
	sudo reboot
kdev_clear_log:
	sudo rm /var/log/messages
	sudo touch /var/log/messages
	sudo /etc/init.d/sysklogd restart
	sudo dmesg -c > /dev/null
kdev_cache_debug:
	for x in /sys/kernel/slab/cache_irp/*; do echo $$x; cat $$x;done
	for x in /sys/kernel/slab/cache_buf/*; do echo $$x; cat $$x;done
# This data is take either from the windows inf file, the linux translated
# PCI device id table or lspci on a machine with the hardware...
KDEV_PCI_VENDOR:=0x10B5
KDEV_PCI_DEVICE:=0x9056
KDEV_PCI_SUBVENDOR:=0x10B5
KDEV_PCI_SUBDEVICE:=0x9056
kdev_lspci_device:
	sudo lspci -d $(KDEV_PCI_VENDOR):$(KDEV_PCI_DEVICE) -nn -vvv -xxxx
kdev_lspci_all:
	sudo lspci -nn -vvv -xxxx
# our own commands - these have to do with BOTH kernel modules...
# in the next target notice the reverse order: first remove high level driver,
# then lower level. On insmod the reverse: first low level driver and then high level...
kdev_insmod: $(LINUX_MOD)
	-sudo rmmod $(LINUX_NAME)
	sudo insmod $(LINUX_MOD)
	sleep 1
	-sudo chmod 666 /dev/Win /dev/KFcb*
kdev_lsmod: $(LINUX_MOD)
	sudo lsmod | grep $(LINUX_NAME)
kdev_rmmod: $(LINUX_MOD)
	-sudo rmmod $(LINUX_NAME)
kdev_modinfo: $(LINUX_MOD)
	sudo modinfo $(LINUX_MOD)
kdev_modreloc: $(LINUX_MOD)
	objdump -r $(LINUX_MOD)
kdev_modreloc_count:
	objdump -r $(LINUX_MOD) | grep R_386_NONE | wc -l
kdev_nmmod: $(LINUX_MOD)
	nm -C $(LINUX_MOD)
####################################
# SECTION: WIN - libWin and Win.ko #
####################################
# this is where the win files are
WIN_DIR:=win/

WIN_TEST_DIR:=$(WIN_DIR)test/
WIN_TEST_SRCS:=$(shell find $(WIN_TEST_DIR) -name "*.cc")
WIN_TEST_DEPS:=$(call to_deps,$(WIN_TEST_SRCS))
WIN_TEST_BINS:=$(call to_bins,$(WIN_TEST_SRCS))
WIN_TEST_HDRS:=$(shell find $(WIN_TEST_DIR) -name "*.h")

WIN_LIB_DIR:=$(WIN_DIR)lib/
WIN_LIB_SRCS:=$(shell find $(WIN_LIB_DIR) -name "*.c")
WIN_LIB_DEPS:=$(call to_deps,$(WIN_LIB_SRCS))
WIN_LIB_OBJS:=$(call to_objs,$(WIN_LIB_SRCS))
WIN_LIB_HDRS:=$(shell find $(WIN_LIB_DIR) -name "*.h")

# some windows library definitions
WIN_LIB_NAME:=Win
WIN_LIB_A_NAME:=$(call to_ar,$(WIN_LIB_NAME))
WIN_LIB_SO_NAME:=$(call to_lib,$(WIN_LIB_NAME))

# these are the include flags if you want user space support
WIN_INC_FLAGS:=-I$(WIN_LIB_DIR) -I$(WIN_LIB_DIR)fake $(SHARED_INC) $(LINUX_INC)
# these are the flags for someone wanting to link with us
WIN_LINK:=$(call link_to,$(WIN_LIB_NAME))

# gather to WIN_ALL
WIN_SRCS_ALL:=$(WIN_TEST_SRCS) $(WIN_LIB_SRCS)
WIN_DEPS_ALL:=$(WIN_TEST_DEPS) $(WIN_LIB_DEPS)
WIN_OBJS_ALL:=$(WIN_LIB_OBJS)
WIN_HDRS_ALL:=$(WIN_TEST_HDRS) $(WIN_LIB_HDRS)
WIN_LIBS_ALL:=$(WIN_LIB_SO_NAME)
WIN_ARXS_ALL:=$(WIN_LIB_A_NAME)
WIN_BINS_ALL:=$(WIN_TEST_BINS)

# this includes ONLY targets
WIN_ALL:=$(WIN_DEPS_ALL) $(WIN_OBJS_ALL) $(WIN_LIBS_ALL) $(WIN_ARXS_ALL) $(WIN_BINS_ALL)

# add all the targets
ALL_SRCS+=$(WIN_SRCS_ALL)
ALL_MSRC+=$(WIN_SRCS_ALL)
ALL_DEPS+=$(WIN_DEPS_ALL)
ALL_OBJS+=$(WIN_OBJS_ALL)
ALL_HDRS+=$(WIN_HDRS_ALL)
ALL_MHDR+=$(WIN_HDRS_ALL)
ALL_LIBS+=$(WIN_LIBS_ALL)
ALL_ARXS+=$(WIN_ARXS_ALL)
ALL_BINS+=$(WIN_BINS_ALL)

# rules for compiling user space support library
$(WIN_LIB_OBJS): $(OBJP)objs/%.o: %.c $(ALL_MKDEPS)
	$(call do_mkdbg,win_objs,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(WIN_INC_FLAGS)
$(WIN_LIB_DEPS): $(OBJP)deps/%.d: %.c $(ALL_MKDEPS)
	$(call do_mkdbg,win_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(CFLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $< $(WIN_INC_FLAGS)
$(WIN_LIB_A_NAME): $(WIN_LIB_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,win_ar,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(AR_TOOL) $@ $(WIN_OBJS_ALL)
$(WIN_LIB_SO_NAME): $(WIN_LIB_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,win_so,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(CFLAGS) -shared -o $@ $(WIN_OBJS_ALL) $(LINK_PTHREAD)

# rules for compiling the test binaries...
$(WIN_TEST_DEPS): $(OBJP)deps/%.d: %.cc $(ALL_MKDEPS)
	$(call do_mkdbg,win_test_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $< $(WIN_INC_FLAGS)
$(WIN_TEST_BINS): $(OBJP)bin/%: $(WIN_TEST_DIR)%.cc $(ALL_MKDEPS) $(WIN_LIB_SO_NAME)
	$(call do_mkdbg,win_test_exe,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(WIN_INC_FLAGS) $(WIN_LINK)

# fake targets
.PHONY: win_all win_clean win_debug
win_all: $(WIN_ALL)
win_clean:
	$(call do_smkdbg,win_clean)
	-$(Q)rm -f $(WIN_ALL)
	-$(Q)find $(OBJP) -type d -and -empty -delete
win_debug:
	$(info WIN_SRCS_ALL is $(WIN_SRCS_ALL))
	$(info WIN_DEPS_ALL is $(WIN_DEPS_ALL))
	$(info WIN_OBJS_ALL is $(WIN_OBJS_ALL))
	$(info WIN_HDRS_ALL is $(WIN_HDRS_ALL))
	$(info WIN_LIBS_ALL is $(WIN_LIBS_ALL))
	$(info WIN_ARXS_ALL is $(WIN_ARXS_ALL))
	$(info WIN_BINS_ALL is $(WIN_BINS_ALL))
	$(info WIN_MISC_ALL is $(WIN_MISC_ALL))
	$(info WIN_INC_FLAGS is $(WIN_INC_FLAGS))
	$(info WIN_LINK is $(WIN_LINK))
	$(info WIN_TEST_DEPS is $(WIN_TEST_DEPS))
###################################
# SECTION: CTST - various C tests #
###################################
# this is where the c test files are
CTST_DIR:=ctst/

CTST_SRCS_ALL:=$(shell find $(CTST_DIR) -name "*.c")
CTST_DEPS_ALL:=$(call to_deps,$(CTST_SRCS_ALL))
CTST_BINS_ALL:=$(call to_bins,$(CTST_SRCS_ALL))
CTST_HDRS_ALL:=$(shell find $(CTST_DIR) -name "*.h")

# these are the include flags to compile the c tests
CTST_INC_FLAGS:=-I$(CTST_DIR)

# this includes ONLY targets
CTST_ALL:=$(CTST_DEPS_ALL) $(CTST_BINS_ALL)

# add all the targets
ALL_SRCS+=$(CTST_SRCS_ALL)
ALL_MSRC+=$(CTST_SRCS_ALL)
ALL_DEPS+=$(CTST_DEPS_ALL)
ALL_BINS+=$(CTST_BINS_ALL)

# rules for compiling the test binaries...
$(CTST_DEPS_ALL): $(OBJP)deps/%.d: %.c $(ALL_MKDEPS)
	$(call do_mkdbg,ctst_test_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(CFLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $< $(CTST_INC_FLAGS)
$(CTST_BINS_ALL): $(OBJP)bin/%: $(CTST_DIR)%.c $(ALL_MKDEPS)
	$(call do_mkdbg,ctst_test_exe,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(CTST_INC_FLAGS) -lpthread -lrt

# fake targets
.PHONY: ctst_all ctst_clean ctst_debug
ctst_all: $(CTST_ALL)
ctst_clean:
	$(call do_smkdbg,ctst_clean)
	-$(Q)rm -f $(CTST_ALL)
	-$(Q)find $(OBJP) -type d -and -empty -delete
ctst_debug:
	$(info CTST_SRCS_ALL is $(CTST_SRCS_ALL))
	$(info CTST_DEPS_ALL is $(CTST_DEPS_ALL))
	$(info CTST_BINS_ALL is $(CTST_BINS_ALL))
	$(info CTST_INC_FLAGS is $(CTST_INC_FLAGS))
##########################################
# SECTION: WINDLL - windows dll creation #
##########################################
WINDLL_LIBS_ALL:=
WINDLL_ARXS_ALL:=
WINDLL_OBJS_ALL:=
WINDLL_DEPS_ALL:=
WINDLL_HDRS_ALL:=
WINDLL_BINS_ALL:=
define template_windll
# this is the directory in which we will find windows dlls
WINDLL_SRCS_$(1):=$$(NUMAKE_WINDLL_SRCS_$(1))
WINDLL_HDRS_$(1):=$$(NUMAKE_WINDLL_HDRS_$(1))
WINDLL_INCLUDE_$(1):=$$(NUMAKE_WINDLL_INCLUDE_$(1))
WINDLL_LINK_BASES_$(1):=$$(NUMAKE_WINDLL_LINK_BASES_$(1))
WINDLL_ELINK_BASES_$(1):=$$(NUMAKE_WINDLL_ELINK_BASES_$(1))

WINDLL_LINK_$(1):=$$(call link_to,$$(WINDLL_LINK_BASES_$(1))) $$(WIN_LINK)
WINDLL_ELINK_$(1):=$$(call elink_to,$$(WINDLL_ELINK_BASES_$(1)))
WINDLL_LINKME_$(1):=$$(call link_to,$(1))
WINDLL_DEPLIBS_$(1):=$$(call to_lib,$$(WINDLL_LINK_BASES_$(1)))
WINDLL_A_NAME_$(1):=$$(call to_ar,$(1))
WINDLL_SO_NAME_$(1):=$$(call to_lib,$(1))
WINDLL_BINS_$(1):=$$(call to_bin,Test_Dlllink_$(1))
WINDLL_LIBS_$(1):=$$(WINDLL_SO_NAME_$(1))
WINDLL_ARXS_$(1):=$$(WINDLL_A_NAME_$(1))
# this is a flags parameters to be used to include win files
WINDLL_INC_FLAGS_$(1):=$$(addprefix -I,$$(WINDLL_INCLUDE_$(1))) $$(WIN_INC_FLAGS)
# these are the windows dll objects
WINDLL_OBJS_$(1):=$$(call to_objs,$$(WINDLL_SRCS_$(1)))
# these are the windows dependency files
WINDLL_DEPS_$(1):=$$(call to_deps,$$(WINDLL_SRCS_$(1)))
# add the windll targets
WINDLL_LIBS_ALL+=$$(WINDLL_LIBS_$(1))
WINDLL_ARXS_ALL+=$$(WINDLL_ARXS_$(1))
WINDLL_OBJS_ALL+=$$(WINDLL_OBJS_$(1))
WINDLL_DEPS_ALL+=$$(WINDLL_DEPS_$(1))
WINDLL_HDRS_ALL+=$$(WINDLL_HDRS_$(1))
WINDLL_BINS_ALL+=$$(WINDLL_BINS_$(1))

$$(WINDLL_OBJS_$(1)): $$(OBJP)objs/%.o: %.cpp $$(ALL_MKDEPS)
	$$(call do_mkdbg,windll_objs,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(CXXFLAGS) -c -o $$@ $$< $$(WINDLL_INC_FLAGS_$(1))
$$(WINDLL_DEPS_$(1)): $$(OBJP)deps/%.d: %.cpp $$(ALL_MKDEPS)
	$$(call do_mkdbg,windll_deps,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(CXXFLAGS) $$(DEPS_FLAGS) $$(call to_objs,$$<) -o $$@ $$< $$(WINDLL_INC_FLAGS_$(1))
$$(WINDLL_A_NAME_$(1)): $$(WINDLL_OBJS_$(1)) $$(ALL_MKDEPS)
	$$(call do_mkdbg,windll_ar,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(AR_TOOL) $$@ $$(WINDLL_OBJS_$(1))
$$(WINDLL_SO_NAME_$(1)): $$(WINDLL_OBJS_$(1)) $$(WINDLL_DEPLIBS_$(1)) $$(ALL_MKDEPS)
	$$(call do_mkdbg,windll_so,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(CXXFLAGS) -shared -o $$@ $$(WINDLL_OBJS_$(1)) $$(WINDLL_LINK_$(1)) $$(WINDLL_ELINK_$(1))
$$(WINDLL_BINS_$(1)): $$(WINDLL_SO_NAME_$(1)) $$(ALL_MKDEPS)
	$$(call do_mkdbg,windll_bins,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(CXXFLAGS) $$(LDFLAGS) -o $$@ $$(TEST_EMPTY) $$(WINDLL_LINKME_$(1)) $$(WINDLL_ELINK_$(1))
endef
# foreach loop must be RIGHT AFTER the template for you to use template
# products later on...
WINDLL_NAMES:=$(NUMAKE_WINDLL_NAMES)
$(foreach name,$(WINDLL_NAMES),$(eval $(call template_windll,$(name))))

# add to all
ALL_LIBS+=$(WINDLL_LIBS_ALL)
ALL_ARXS+=$(WINDLL_ARXS_ALL)
ALL_OBJS+=$(WINDLL_OBJS_ALL)
ALL_DEPS+=$(WINDLL_DEPS_ALL)
ALL_HDRS+=$(WINDLL_HDRS_ALL)
ALL_MHDR+=$(WINDLL_HDRS_ALL)
# these are NOT my headers
ALL_HDRS+=$(WINDLL_HDRS_ALL)
ALL_BINS+=$(WINDLL_BINS_ALL)

# fake targets
.PHONY: windll_all windll_clean winall_debug
windll_all: $(WINDLL_LIBS_ALL) $(WINDLL_OBJS_ALL) $(WINDLL_DEPS_ALL) $(WINDLL_BINS_ALL)
windll_clean:
	-$(Q)rm -f $(WINDLL_LIBS_ALL) $(WINDLL_OBJS_ALL) $(WINDLL_DEPS_ALL) $(WINDLL_BINS_ALL)
	$(Q)find $(OBJP) -type d -and -empty -delete
windll_debug:
	$(info WINDLL_NAMES is $(WINDLL_NAMES))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_SRCS_$(name) is $(WINDLL_SRCS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_HDRS_$(name) is $(WINDLL_HDRS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_INC_FLAGS_$(name) is $(WINDLL_INC_FLAGS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_LINK_$(name) is $(WINDLL_LINK_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_OBJS_$(name) is $(WINDLL_OBJS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_DEPS_$(name) is $(WINDLL_DEPS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_LIBS_$(name) is $(WINDLL_LIBS_$(name))))
	$(foreach name,$(WINDLL_NAMES),$(info WINDLL_BINS_$(name) is $(WINDLL_BINS_$(name))))
	$(info WINDLL_BINS_ALL is $(WINDLL_BINS_ALL))
	$(info WINDLL_LIBS_ALL is $(WINDLL_LIBS_ALL))
	$(info WINDLL_OBJS_ALL is $(WINDLL_OBJS_ALL))
	$(info WINDLL_DEPS_ALL is $(WINDLL_DEPS_ALL))
##########################################
# SECTION: WINEXE - windows exe creation #
##########################################
WINEXE_BINS_ALL:=
WINEXE_DEPS_ALL:=
define template_winexe
WINEXE_SRCS_$(1):=$$(NUMAKE_WINEXE_SRCS_$(1))
WINEXE_INCLUDE_$(1):=$$(NUMAKE_WINEXE_INCLUDE_$(1))
WINEXE_LINK_BASES_$(1):=$$(NUMAKE_WINEXE_LINK_BASES_$(1))
WINEXE_ELINK_BASES_$(1):=$$(NUMAKE_WINEXE_ELINK_BASES_$(1))

WINEXE_LINK_$(1):=$$(call link_to,$$(WINEXE_LINK_BASES_$(1)))
WINEXE_ELINK_$(1):=$$(call elink_to,$$(WINEXE_ELINK_BASES_$(1)))
WINEXE_DEPLIBS_$(1):=$$(call to_lib,$$(WINEXE_LINK_BASES_$(1)))
# this is a flags parameters to be used to include win files
WINEXE_INC_FLAGS_$(1):=$$(addprefix -I,$$(WINEXE_INCLUDE_$(1))) $$(WIN_INC_FLAGS)
# these are the windows executables
WINEXE_BINS_$(1):=$$(OBJP)bin/$(1)
# these are the windows dependency files
WINEXE_DEPS_$(1):=$$(call to_deps,$$(WINEXE_SRCS_$(1)))
# add to all
WINEXE_BINS_ALL+=$$(WINEXE_BINS_$(1))
WINEXE_DEPS_ALL+=$$(WINEXE_DEPS_$(1))

# rules for compiling user space windows apps
$$(WINEXE_BINS_$(1)): $$(WINEXE_SRCS_$(1)) $$(WINEXE_DEPLIBS_$(1)) $$(ALL_MKDEPS)
	$$(call do_mkdbg,winexe_bins,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(LDFLAGS) $$(CXXFLAGS) -o $$@ $$< $$(WINEXE_INC_FLAGS_$(1)) $$(WINEXE_LINK_$(1)) $$(WINEXE_ELINK_$(1))
$$(WINEXE_DEPS_$(1)): $$(WINEXE_SRCS_$(1)) $$(ALL_MKDEPS)
	$$(call do_mkdbg,winexe_deps,$$@)
	$$(Q)$$(MKDIR) $$(@D)
	$$(Q)$$(CXX) $$(LDFLAGS) $$(CXXFLAGS) $$(DEPS_FLAGS) $$(call to_objs,$$<) -o $$@ $$< $$(WINEXE_INC_FLAGS_$(1))
endef
WINEXE_NAMES:=$(NUMAKE_WINEXE_NAMES)
$(foreach name,$(WINEXE_NAMES),$(eval $(call template_winexe,$(name))))

# add to all
ALL_BINS+=$(WINEXE_BINS_ALL)
ALL_DEPS+=$(WINEXE_DEPS_ALL)

# fake targets
.PHONY: winexe_all winexe_clean winexe_debug
winexe_all: $(WINEXE_BINS_ALL) $(WINEXE_DEPS_ALL)
winexe_clean:
	-$(Q)rm -f $(WINEXE_BINS_ALL) $(WINEXE_DEPS_ALL)
	$(Q)find $(OBJP) -type d -and -empty -delete
winexe_debug:
	$(info WINEXE_BINS_ALL is $(WINEXE_BINS_ALL))
	$(info WINEXE_DEPS_ALL is $(WINEXE_DEPS_ALL))
###################################
# SECTION: TEST - testing section #
###################################
# testing targets
# notice that the running testing user space applications
# DOES NOT require root permission. This means that the device
# that is created in /dev should give permissions to access it
# to user space.
# fake targets
.PHONY: test_insmod test_stress_insmod_rmmod test_stress_insrm test_Test_Console_ReadWriteOneChannel test_Test_Console_WaitForInterrupt test_Test_Console_WaitForInterruptEx test_Test_Gen_Simple test test_comm_0 test_comm_1 test_comm_2 test_comm_3 test_stress
test_insmod: $(LINUX_MOD)
	-sudo rmmod $(LINUX_NAME)
	sudo dmesg -c > /dev/null
	-sudo insmod $(LINUX_MOD)
	sudo dmesg | c++filt
test_stress_insmod_rmmod: $(LINUX_MOD)
	-sudo rmmod $(LINUX_NAME)
	./scripts/test_stress_insmod_rmmod.pl $(LINUX_MOD) 1000
test_stress_insrm:
	./scripts/test_stress_insmod_rmmod.pl $(MOD) 1000
test_Test_Console_ReadWriteOneChannel:
	./obj/bin/Test_Console_ReadWriteOneChannel
test_Test_Console_WaitForInterrupt:
	./obj/bin/Test_Console_WaitForInterrupt
test_Test_Console_WaitForInterruptEx:
	./obj/bin/Test_Console_WaitForInterruptEx
test_Test_Gen_Simple:
	./obj/bin/Test_Gen_Simple
test: test_Test_Console_ReadWriteOneChannel
test_comm_0:
	./obj/bin/Test_Console_ReadWriteOneChannel -chan 0 -FullDuplexMode 1
test_comm_1:
	./obj/bin/Test_Console_ReadWriteOneChannel -chan 1 -FullDuplexMode 1
test_comm_2:
	./obj/bin/Test_Console_ReadWriteOneChannel -chan 2 -FullDuplexMode 1
test_comm_3:
	./obj/bin/Test_Console_ReadWriteOneChannel -chan 3 -FullDuplexMode 1
test_cui_0:
	./obj/bin/Test_Console_UI -Chan 0 -FullDuplexMode 1
test_cui_1:
	./obj/bin/Test_Console_UI -Chan 1 -FullDuplexMode 1
test_cui_10:
	./obj/bin/Test_Console_UI -Chan 10 -FullDuplexMode 1
test_cui_11:
	./obj/bin/Test_Console_UI -Chan 11 -FullDuplexMode 1
test_slave:
	./obj/bin/Test_Slave -FullDuplexMode 1
test_stress:
	./scripts/test_stress_cmd.pl 100 ./obj/bin/Test_Console_ReadWriteOneChannel 1
#################################################
# SECTION: GRA - graphical applications section #
#################################################
GRA_DIR:=driver_apps/
GRA_CXX_FLAGS:=-I$(GRA_DIR)DriverTools_Gen -I$(GRA_DIR)CommonFiles -I$(GRA_DIR)FcbComm_MonitorWx $(WX_CPP) $(WIN_INC_FLAGS) $(LINUX_INC) $(SHARED_INC) -Idriver/Dlls+Tests/General/Dll_FcbGeneral -Idriver/Drivers/KFcbGen -Idriver/Drivers -I$(GRA_DIR)Extensions -Idriver/Dlls+Tests/Bus/Dll_FcbDma -Idriver/Drivers/KFcbBus -Idrivers/Fcb/Drivers/KFcbComm -Idrivers/Fcb/Dlls+Tests/Comm/Dll_FcbComm -Idrivers/Fcb/Dlls+Tests/Comm/Specifics_DLL -Idrivers/Fcb/Dlls+Tests/Lvds/FcbPicturesDll -Idrivers/Fcb/Drivers/KFcbLvds
GRA_LD_FLAGS:=\
	$(WX_LD)\
	$(call link_to,FcbDma)\
	$(call link_to,FcbGeneral)\
	$(call link_to,FcbComm)\
	$(call link_to,FcbPictures)
GRA_LD_FLAGS_STATIC:=\
	$(WX_LD)\
	-static\
	$(call link_to,FcbDma)\
	$(call link_to,FcbGeneral)\
	$(call link_to,FcbComm)\
	$(call link_to,FcbPictures)

GRA_EXT_SRC_DIR:=$(GRA_DIR)Extensions/
GRA_EXT_SRCS:=$(shell find $(GRA_EXT_SRC_DIR) -name "*.cpp" -type f)
GRA_EXT_HDRS:=$(shell find $(GRA_EXT_SRC_DIR) -name "*.h" -type f)
GRA_EXT_OBJS:=$(call to_objs,$(GRA_EXT_SRCS))
GRA_EXT_DEPS:=$(call to_deps,$(GRA_EXT_SRCS))

GRA_CMN_SRC_DIR:=$(GRA_DIR)CommonFiles/
GRA_CMN_SRCS:=$(shell find $(GRA_CMN_SRC_DIR) -name "*.cpp" -type f)
GRA_CMN_HDRS:=$(shell find $(GRA_CMN_SRC_DIR) -name "*.h" -type f)
GRA_CMN_OBJS:=$(call to_objs,$(GRA_CMN_SRCS))
GRA_CMN_DEPS:=$(call to_deps,$(GRA_CMN_SRCS))

GRA_GEN_SRC_DIR:=$(GRA_DIR)DriverTools_Gen/
GRA_GEN_SRCS:=$(shell find $(GRA_GEN_SRC_DIR) -name "*.cpp" -type f)
GRA_GEN_HDRS:=$(shell find $(GRA_GEN_SRC_DIR) -name "*.h" -type f)
GRA_GEN_OBJS:=$(call to_objs,$(GRA_GEN_SRCS))
GRA_GEN_DEPS:=$(call to_deps,$(GRA_GEN_SRCS))
GRA_GEN_BIN_BASENAME:=uiGen
GRA_GEN_BIN_FILE_DYN:=$(call to_bin,$(GRA_GEN_BIN_BASENAME))
GRA_GEN_BIN_FILE_STA:=$(call to_sbin,$(GRA_GEN_BIN_BASENAME))
GRA_GEN_BIN_FOLDER:=$(OBJP)$(GRA_GEN_BIN_BASENAME).folder

GRA_COM_SRC_DIR:=$(GRA_DIR)FcbComm_MonitorWx/
GRA_COM_SRCS:=$(shell find $(GRA_COM_SRC_DIR) -name "*.cpp" -type f)
GRA_COM_HDRS:=$(shell find $(GRA_COM_SRC_DIR) -name "*.h" -type f)
GRA_COM_OBJS:=$(call to_objs,$(GRA_COM_SRCS))
GRA_COM_DEPS:=$(call to_deps,$(GRA_COM_SRCS))
GRA_COM_BIN_BASENAME:=uiCom
GRA_COM_BIN_FILE_DYN:=$(call to_bin,$(GRA_COM_BIN_BASENAME))
GRA_COM_BIN_FILE_STA:=$(call to_sbin,$(GRA_COM_BIN_BASENAME))
GRA_COM_BIN_FOLDER:=$(OBJP)$(GRA_COM_BIN_BASENAME).folder

GRA_PIC_SRC_DIR:=$(GRA_DIR)DriverTools_Pictures/
GRA_PIC_SRCS:=$(shell find $(GRA_PIC_SRC_DIR) -name "*.cpp" -type f)
GRA_PIC_HDRS:=$(shell find $(GRA_PIC_SRC_DIR) -name "*.h" -type f)
GRA_PIC_OBJS:=$(call to_objs,$(GRA_PIC_SRCS))
GRA_PIC_DEPS:=$(call to_deps,$(GRA_PIC_SRCS))
GRA_PIC_BIN_BASENAME:=uiPic
GRA_PIC_BIN_FILE_DYN:=$(call to_bin,$(GRA_PIC_BIN_BASENAME))
GRA_PIC_BIN_FILE_STA:=$(call to_sbin,$(GRA_PIC_BIN_BASENAME))
GRA_PIC_BIN_FOLDER:=$(OBJP)$(GRA_PIC_BIN_BASENAME).folder

GRA_HEL_SRC_DIR:=$(GRA_DIR)DriverTools_Hel/
GRA_HEL_SRCS:=$(shell find $(GRA_HEL_SRC_DIR) -name "*.cpp" -type f)
GRA_HEL_HDRS:=$(shell find $(GRA_HEL_SRC_DIR) -name "*.h" -type f)
GRA_HEL_OBJS:=$(call to_objs,$(GRA_HEL_SRCS))
GRA_HEL_DEPS:=$(call to_deps,$(GRA_HEL_SRCS))
GRA_HEL_BIN_BASENAME:=uiHel
GRA_HEL_BIN_FILE_DYN:=$(call to_bin,$(GRA_HEL_BIN_BASENAME))
GRA_HEL_BIN_FILE_STA:=$(call to_sbin,$(GRA_HEL_BIN_BASENAME))
GRA_HEL_BIN_FOLDER:=$(OBJP)$(GRA_HEL_BIN_BASENAME).folder

GRA_SRCS:=$(GRA_GEN_SRCS) $(GRA_COM_SRCS) $(GRA_PIC_SRCS) $(GRA_HEL_SRCS) $(GRA_EXT_SRCS) $(GRA_CMN_SRCS)
GRA_HDRS:=$(GRA_GEN_HDRS) $(GRA_COM_HDRS) $(GRA_PIC_HDRS) $(GRA_HEL_HDRS) $(GRA_EXT_HDRS) $(GRA_CMN_HDRS)
GRA_DEPS:=$(GRA_GEN_DEPS) $(GRA_COM_DEPS) $(GRA_PIC_DEPS) $(GRA_HEL_DEPS) $(GRA_EXT_DEPS) $(GRA_CMN_DEPS)
GRA_OBJS:=$(GRA_GEN_OBJS) $(GRA_COM_OBJS) $(GRA_PIC_OBJS) $(GRA_HEL_OBJS) $(GRA_EXT_OBJS) $(GRA_CMN_OBJS)
GRA_BINS:=$(GRA_GEN_BIN_FILE_DYN) $(GRA_COM_BIN_FILE_DYN) $(GRA_HEL_BIN_FILE_DYN) $(GRA_PIC_BIN_FILE_DYN)
ifeq ($(DO_STATIC),1)
GRA_BINS+=$(GRA_GEN_BIN_FILE_STA) $(GRA_COM_BIN_FILE_STA) $(GRA_HEL_BIN_FILE_STA) $(GRA_PIC_BIN_FILE_STA)
endif # DO_STATIC
GRA_FOLD:=$(OBJP)folder
GRA_FOLD_STAMP:=$(GRA_FOLD).stamp
GRA_MISC:=$(GRA_FOLD_STAMP)
GRA_EFLD:=graphical/

# all graphical stuff
GRA_ALL:=$(GRA_FOLD) $(GRA_OBJS) $(GRA_DEPS) $(GRA_BINS)

# add to all
ifeq ($(DO_GRA),1)
ALL_SRCS+=$(GRA_SRCS)
ALL_MSRC+=$(GRA_SRCS)
ALL_MHDR+=$(GRA_HDRS)
# these should not be delivered to clients
#ALL_HDRS+=$(GRA_HDRS)
ALL_EFLD+=$(GRA_EFLD)
ALL_OBJS+=$(GRA_OBJS)
ALL_DEPS+=$(GRA_DEPS)
ALL_BINS+=$(GRA_BINS)
ALL_MISC+=$(GRA_MISC)
ALL_MISC_DEFAULT+=$(GRA_MISC)
ALL_CFLD+=$(GRA_FOLD)
endif # DO_GRA

# rules
$(GRA_OBJS): $(OBJP)objs/%.o: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,gra_objs,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(CXXFLAGS) -c -o $@ $< $(GRA_CXX_FLAGS)
$(GRA_DEPS): $(OBJP)deps/%.d: %.cpp $(ALL_MKDEPS)
	$(call do_mkdbg,gra_deps,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(CXXFLAGS) $(DEPS_FLAGS) $(call to_objs,$<) -o $@ $< $(GRA_CXX_FLAGS)
$(GRA_GEN_BIN_FILE_DYN): $(GRA_GEN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_gen_link_dynamic,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_GEN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS)
$(GRA_COM_BIN_FILE_DYN): $(GRA_COM_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_com_link_dynamic,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_COM_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS)
$(GRA_PIC_BIN_FILE_DYN): $(GRA_PIC_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_pic_link_dynamic,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_PIC_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS)
$(GRA_HEL_BIN_FILE_DYN): $(GRA_HEL_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_hel_link_dynamic,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_HEL_OBJS) $(GRA_LD_FLAGS)
$(GRA_GEN_BIN_FILE_STA): $(GRA_GEN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_gen_link_static,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_GEN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS_STATIC)
$(GRA_COM_BIN_FILE_STA): $(GRA_COM_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_com_link_static,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_COM_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS_STATIC)
$(GRA_PIC_BIN_FILE_STA): $(GRA_PIC_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_pic_link_static,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_PIC_OBJS) $(GRA_CMN_OBJS) $(GRA_EXT_OBJS) $(GRA_LD_FLAGS_STATIC)
$(GRA_HEL_BIN_FILE_STA): $(GRA_HEL_OBJS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_hel_link_static,$@)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(UI_CXX) $(LDFLAGS) -o $@ $(GRA_HEL_OBJS) $(GRA_LD_FLAGS_STATIC)
$(GRA_GEN_BIN_FOLDER): $(GRA_GEN_BIN_FILE_DYN) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_gen_link_static,$@)
	$(Q)-rm -rf $(GRA_GEN_BIN_FOLDER)
	$(Q)$(MKDIR) $(GRA_GEN_BIN_FOLDER)
	$(Q)./scripts/dynamic_collect.pl $(GRA_GEN_BIN_FOLDER) $(GRA_GEN_BIN_FILE_DYN)
$(GRA_COM_BIN_FOLDER): $(GRA_COM_BIN_FILE_DYN) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_com_link_static,$@)
	$(Q)-rm -rf $(GRA_COM_BIN_FOLDER)
	$(Q)$(MKDIR) $(GRA_COM_BIN_FOLDER)
	$(Q)./scripts/dynamic_collect.pl $(GRA_COM_BIN_FOLDER) $(GRA_COM_BIN_FILE_DYN)
$(GRA_PIC_BIN_FOLDER): $(GRA_PIC_BIN_FILE_DYN) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_pic_link_static,$@)
	$(Q)-rm -rf $(GRA_PIC_BIN_FOLDER)
	$(Q)$(MKDIR) $(GRA_PIC_BIN_FOLDER)
	$(Q)./scripts/dynamic_collect.pl $(GRA_PIC_BIN_FOLDER) $(GRA_PIC_BIN_FILE_DYN)
$(GRA_FOLD_STAMP): $(GRA_BINS) $(ALL_MKDEPS)
	$(call do_mkdbg,gra_folder,$@)
	$(Q)-rm -rf $(GRA_FOLD)
	$(Q)$(MKDIR) $(GRA_FOLD)
	$(Q)./scripts/dynamic_collect.pl $(GRA_FOLD) $(GRA_BINS)
	$(Q)touch $(GRA_FOLD_STAMP)

# fake targets
.PHONY: gra_debug gra_clean gra_all
gra_debug:
	$(info GRA_GEN_SRCS is $(GRA_GEN_SRCS))
	$(info GRA_GEN_DEPS is $(GRA_GEN_DEPS))
	$(info GRA_GEN_OBJS is $(GRA_GEN_OBJS))
	$(info GRA_COM_SRCS is $(GRA_COM_SRCS))
	$(info GRA_COM_DEPS is $(GRA_COM_DEPS))
	$(info GRA_COM_OBJS is $(GRA_COM_OBJS))
	$(info GRA_EXT_SRCS is $(GRA_EXT_SRCS))
	$(info GRA_EXT_DEPS is $(GRA_EXT_DEPS))
	$(info GRA_EXT_OBJS is $(GRA_EXT_OBJS))
	$(info GRA_BINS is $(GRA_BINS))
	$(info GRA_OBJS is $(GRA_OBJS))
	$(info GRA_SRCS is $(GRA_SRCS))
	$(info GRA_HDRS is $(GRA_HDRS))
	$(info GRA_DEPS is $(GRA_DEPS))
gra_clean:
	-$(Q)rm -f $(GRA_OBJS) $(GRA_DEPS) $(GRA_BINS)
	-$(Q)rm -rf $(GRA_FOLD)
gra_all: $(GRA_ALL)
#########################################
# SECTION: MISC - miscelleneous section #
#########################################
MISC_TAGS:=tags
# fake targets
MISC_PHONY:=misc_sshd_start misc_sshd_stop misc_up_interface misc_ping_target misc_connect_target misc_push_target misc_add_me_to_root_group misc_sloc misc_tags misc_find_broken_symlinks misc_target_debug
.PHONY: $(MISC_PHONY)
ifneq ($(filter $(MISC_PHONY),$(MAKECMDGOALS)),)
DO_INCLUDE:=0
endif
misc_sshd_start:
	sudo /etc/init.d/ssh start
misc_sshd_stop:
	sudo /etc/init.d/ssh stop
misc_up_interface:
	-sudo ifdown eth1
	-sudo ifconfig eth1 down
	-sudo ifdown eth0
	-sudo ifconfig eth0 down
misc_ping_target:
	ping target
misc_connect_target:
	@rm -f $$HOME/.ssh/known_hosts
	@./scripts/ssh.ex root@target
misc_push_target:
	@rm -f $$HOME/.ssh/known_hosts
	@./scripts/scp.ex -r $$PWD root@target:~
misc_push_target_lite:
	@rm -f $$HOME/.ssh/known_hosts
	@./scripts/scp.ex -r $$PWD/obj/modules $$PWD/obj/bin $$PWD/obj/lib $$PWD/target_env $$PWD/obj/folder root@target:~
misc_push_target_lite_lite:
	@rm -f $$HOME/.ssh/known_hosts
	@./scripts/scp.ex -r $$PWD/obj/modules $$PWD/obj/bin $$PWD/obj/lib $$PWD/target_env root@target:~
misc_add_me_to_root_group:
	sudo adduser $$USER root
misc_sloc:
	sloccount Makefile configs/ nu/ kernel_module/ win/ demo/ shared/ scripts/ target_env/ driver_apps/ ctst/
misc_tags: $(MISC_TAGS)
misc_find_broken_symlinks:
	find . -type l | (while read FN ; do test -e "$$FN" || ls -ld "$$FN"; done)
misc_target_debug:
	gdb ./obj/bin/Test_Console_PicturesRx -tui -x misc/gdbinit -c core
$(MISC_TAGS): $(ALL_SRCS) $(ALL_HDRS)
	$(call do_mkdbg,generating ctags,$@)
	$(Q)ctags $(ALL_SRCS) $(ALL_HDRS)
# add to all
ifeq ($(DO_CTAGS),1)
ALL_MISC+=$(MISC_TAGS)
ALL_MISC_DEFAULT+=$(MISC_TAGS)
endif # DO_CTAGS
################################
# SECTION: SQ - source quality #
################################
# fake targets
.PHONY: sq_find_test sq_find_badstuff sq_find_exe sq_find_junk
SQ_FIND:=Zmau
SQ_FIND:=DO_TIMING
SQ_FIND:=HACKDOM
sq_find_text:
	fgrep $(SQ_FIND) `find . -name "*.cpp" -or -name "*.h" -or -name "*.c"`
sq_find_badstuff:
	$(info XXXXXXXXXXXX tabs in code XXXXXXXXXXXXXXXX)
	-@grep "[a-zA-Z;,)]	" $(ALL_MSRC) $(ALL_MHDR)
	$(info XXXXXXXXXXXX double spaces XXXXXXXXXXXXXXXX)
	-@grep "\ \ " $(ALL_MSRC) $(ALL_MHDR)
	$(info XXXXXXXXXXXX space at end of line XXXXXXXXXXXXXXXX)
	-@grep " $$" $(ALL_MSRC) $(ALL_MHDR)
sq_find_exe:
	find . -type f -and -executable
sq_find_junk:
	find . -type f -and \( -name "*.d" -or -name "*.o" \)
##############################################
# SECTION: PACK - packaging for distribution #
##############################################
PACK_VERSION:=$(NUMAKE_PACK_VERSION)
PACK_NAME:=$(NUMAKE_PACK_NAME)
PACK_FULLNAME:=$(PACK_NAME)-$(PACK_VERSION)-$(CONFIG_NAME)
PACK_CHANGELOG:=doc/Changelog.txt
PACK_README:=doc/README_LINUX.txt
PACK_TAR:=$(OBJP)$(PACK_FULLNAME).tar
PACK_FILE:=$(PACK_FULLNAME).tar.bz2
PACK_ABSFILE:=$(OBJP)$(PACK_FILE)
PACK_VER:=$(OBJP)ver.txt

# add version to compilation flags
CFLAGS+=-DNUMAKE_PACK_VERSION=$(NUMAKE_PACK_VERSION)
CXXFLAGS+=-DNUMAKE_PACK_VERSION=$(NUMAKE_PACK_VERSION)
KERNEL_KCFLAGS+=-DNUMAKE_PACK_VERSION=$(NUMAKE_PACK_VERSION) -DNUMAKE_PACK_VERSION_MAJ=$(NUMAKE_PACK_VERSION_MAJ) -DNUMAKE_PACK_VERSION_MIN=$(NUMAKE_PACK_VERSION_MIN) -DNUMAKE_PACK_VERSION_PAT=$(NUMAKE_PACK_VERSION_PAT)

# add to all
ALL_DOCS_DELI+=$(PACK_CHANGELOG) $(PACK_README) $(CONFIG_REAL)
ALL_MISC+=$(PACK_ABSFILE) $(PACK_VER)
# do not do packaging by default...
#ALL_MISC_DEFAULT+=$(PACK_ABSFILE) $(PACK_VER)

# rules

$(PACK_VER): $(NUMAKE_FILE)
	$(call do_mkdbg,making version stamp file,$@)
	$(Q)echo "package version $(PACK_VERSION) built by $(USER) on $(HOSTNAME) at $(DATE)" > $@
	$(Q)echo "build machine details are $(UNAMEA)" >> $@
	$(Q)echo "lsb machine details are:" >> $@
	$(Q)lsb_release -a >> $@ 2> /dev/null
	$(Q)echo "default compiler details are:" >> $@
	$(Q)gcc --version >> $@
# you can add the following to ease debugging the trasnformations:
# v for verbose
# --show-transformed-names - to show names after transformation as opposed to default which is
# before...
$(PACK_ABSFILE): $(ALL_DOCS_DELI) $(ALL_LIBS) $(ALL_ARXS) $(ALL_HDRS) $(ALL_MODS) $(ALL_BINS) $(PACK_VER) $(GRA_FOLD_STAMP) $(ALL_MKDEPS)
	$(call do_mkdbg,making distribution,$@)
	$(Q)tar jcf $(PACK_ABSFILE) $(PACK_VER) $(OBJP)bin $(OBJP)lib $(OBJP)ar $(OBJP)modules $(OBJP)folder graphical/ scripts/ --transform "s%^scripts%$(OBJP)scripts%" --transform "s%^graphical%$(OBJP)graphical%" --transform "s%^$(OBJP)%$(PACK_FULLNAME)/%" $(ALL_HDRS) --transform "s%^.*/\(.*\)\.h$$%$(PACK_FULLNAME)/include/\1.h%" $(ALL_DOCS_DELI) --transform "s%^doc/\(.*\)%$(PACK_FULLNAME)/doc/\1%" --dereference --transform "s%^configs/\(.*\)%$(PACK_FULLNAME)/configs/\1%"

# fake targets
.PHONY: pack pack_list pack_debug pack_print pack_version pack_ver
PACK_DONTINCLUDE:=pack_version pack_print
ifneq ($(filter $(PACK_DONTINCLUDE),$(MAKECMDGOALS)),)
DO_INCLUDE:=0
endif
pack: $(PACK_ABSFILE)
pack_list:
	tar jtvf $(PACK_ABSFILE)
pack_debug:
	$(info PACK_ABSFILE is $(PACK_ABSFILE))
# must use echo in the next targets since $(info) leaves more text...
pack_print:
	@echo $(PACK_ABSFILE)
pack_version:
	@echo $(PACK_VERSION)
pack_ver: $(PACK_VER)
##########################################################
# SECTION: ALL - this is where everything comes together #
##########################################################
# fake targets
.PHONY: all
.PHONY: all_bins all_libs all_arxs all_objs all_deps all_test all_mods all_misc all_misc_default
.PHONY: all_debug all_help all_nothing
ALL_CLEAN:=all_clean all_clean_us all_clean_explicit all_clean_leftovers all_clean_ac
.PHONY: $(ALL_CLEAN)
ifneq ($(filter $(ALL_CLEAN),$(MAKECMDGOALS)),)
DO_INCLUDE:=0
endif
all: $(ALL_BINS) $(ALL_LIBS) $(ALL_ARXS) $(ALL_OBJS) $(ALL_DEPS) $(ALL_MODS) $(ALL_MISC_DEFAULT)
all_bins: $(ALL_BINS)
all_libs: $(ALL_LIBS)
all_arxs: $(ALL_ARXS)
all_objs: $(ALL_OBJS)
all_deps: $(ALL_DEPS)
all_mods: $(ALL_MODS)
all_misc: $(ALL_MISC)
all_misc_default: $(ALL_MISC_DEFAULT)
all_debug:
	$(info ALL_BINS is $(ALL_BINS))
	$(info ALL_LIBS is $(ALL_LIBS))
	$(info ALL_ARXS is $(ALL_ARXS))
	$(info ALL_OBJS is $(ALL_OBJS))
	$(info ALL_DEPS is $(ALL_DEPS))
	$(info ALL_MODS is $(ALL_MODS))
	$(info ALL_MISC is $(ALL_MISC))
	$(info ALL_MISC_DEFAULT is $(ALL_MISC_DEFAULT))
	$(info ALL_SRCS is $(ALL_SRCS))
	$(info ALL_EFLD is $(ALL_EFLD))
	$(info ALL_HDRS is $(ALL_HDRS))
	$(info ALL_MHDR is $(ALL_MHDR))
	$(info XXXXXXXXXXXXXX deliverables XXXXXXXXXXXXXXXXX)
	$(info ALL_DOCS_DELI is $(ALL_DOCS_DELI))
	$(info XXXXXXXXXXXXXX misc stuff XXXXXXXXXXXXXXXXX)
	$(info MAKECMDGOALS is $(MAKECMDGOALS))
	$(info CONFIG is $(CONFIG))
	$(info CONFIG_NAME is $(CONFIG_NAME))
	$(info CONFIG_REAL is $(CONFIG_REAL))
	$(info WXCONFIG is [$(WX_CONFIG)])
	$(info KERNEL_KCFLAGS is [$(KERNEL_KCFLAGS)])
all_help:
	$(info help is not implemented. Yet. Meanwhile here is a fortune:)
	$(info )
	fortune
all_nothing:
	$(info doing nothing...)
# the next targets need to be sorted to the various categories...
all_clean: $(ALL_CLEAN_TARGETS) all_clean_us all_clean_explicit all_clean_leftovers all_clean_demos
# remove application CC objects
all_clean_us:
	$(call do_smkdbg,clean_us)
	-$(Q)rm -f $(ALL_MISC) $(ALL_BINS) $(ALL_LIBS) $(ALL_ARXS) $(ALL_OBJS) $(ALL_DEPS) $(ALL_MODS)
all_clean_explicit:
	$(call do_smkdbg,clean_explicit)
	-$(Q)rm -rf $(ALL_CFLD)
	-$(Q)rm -f $(ALL_CFIL)
all_clean_leftovers:
	$(call do_smkdbg,clean_leftovers)
	$(Q)find $(OBJP) -type d -and -empty -delete
all_clean_demos:
	$(call do_smkdbg,clean_demos)
	$(Q)$(MAKE) -C demo/user_space clean MAKEFLAGS=
	$(Q)$(MAKE) -C demo/kernel clean MAKEFLAGS=
all_clean_ac:
	$(call do_smkdbg,clean_ac)
	-$(Q)rm -rf autom4te.cache config.cache config.status config.log aclocal.m4 missing
	-$(Q)rm -rf configure config.sub config.guess install-sh
	-$(Q)rm -rf Makefile
########################################################
# SECTION: INSTALL - installation on the local machine #
########################################################
INSTALL_DIR_BASE:=/usr/
INSTALL_DIR_LIB:=$(INSTALL_DIR_BASE)lib/
INSTALL_DIR_AR:=$(INSTALL_DIR_BASE)lib/
INSTALL_DIR_INC:=$(INSTALL_DIR_BASE)include/

# fake targets
.PHONY: install_libs install_arxs install_hdrs install uninstall_libs uninstall_arxs uninstall_hdrs uninstall
install_libs: $(ALL_LIBS)
	sudo install -m 644 $(ALL_LIBS) $(INSTALL_DIR_LIB)
	sudo ldconfig -n $(INSTALL_DIR_LIB)
install_arxs: $(ALL_ARXS)
	sudo install -m 644 $(ALL_ARXS) $(INSTALL_DIR_AR)
install_hdrs: $(ALL_HDRS)
	sudo install -m 644 $(ALL_HDRS) $(INSTALL_DIR_INC)
install: install_libs install_arxs install_hdrs
uninstall_libs:
	sudo rm $(INSTALL_DIR_LIB) $(ALL_LIBS)
	sudo ldconfig -n $(INSTALL_DIR_LIB)
uninstall_arxs:
	sudo rm $(INSTALL_DIR_AR) $(ALL_ARXS)
uninstall_hdrs:
	sudo rm $(INSTALL_DIR_INC) $(ALL_HDRS)
uninstall: uninstall_libs uninstall_arxs uninstall_hdrs
###################################
# SECTION: WIP - work in progress #
###################################
# fake targets
.PHONY: wip_copy_headers
wip_copy_headers:
	# removing old kernel headers
	rm -rf $(USEKDIR)
	# copying new kernel headers
	cp -rL $(KDIR) $(USEKDIR)
	# applying the kernel headers patch to enable C++
	patch -d $(USEKDIR) -p1 < kh.patch
###############################################
# SECTION: ALIAS - aliases for common targets #
###############################################
# fake targets
.PHONY: clean
ifeq ($(MAKECMDGOALS),clean)
DO_INCLUDE:=0
endif # clean
clean: all_clean
##########################################################
# SECTION: PRINT - getting information from the makefile #
##########################################################
# fake targets
PRINT_PHONY:=print_version print_name
.PHONY: $(PRINT_PHONY)
ifneq ($(filter $(PRINT_PHONY),$(MAKECMDGOALS)),)
DO_INCLUDE:=0
endif
# must use @echo in the next ones since $(info) leaves more text
print_version:
	@echo $(NUMAKE_PACK_VERSION)
print_name:
	@echo $(NUMAKE_PACK_NAME)
#################################################
# SECTION: DEP - dependecy information handling #
#################################################
ifeq ($(DO_INCLUDE),1)
# include the deps files (no warnings)
-include $(ALL_DEPS)
endif # DO_INCLUDE
