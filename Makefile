##############
# parameters #
##############
# directories
US_DIRS:=src/examples src/exercises
KERNEL_DIR:=src/kernel
US_INCLUDE:=src/include
# kernel variables
# version of kernel to build against
KVER?=$(shell uname -r)
# folder of the build folder for the kernel you build against
KDIR?=/lib/modules/$(KVER)/build
# fill in the vervosity level you want for the kernel module compilation process
# V:=1 will give you the command lines used...
# Since we are using ?= for assignment it means that you can just
# set this from the command line and avoid changing the makefile...
V?=0
# extra flags to pass to the kernel module creation process...
# regular kernels do not have -Werror and we want it!
# The problem is that this makes the kernel build system scream at me (it fears I am changing
# the flags in some profound ways). This is what we have wrapper scripts for...
KCFLAGS:=-Werror
# do you want dependency on the makefile itself ?!?
DO_ALL_DEPS:=1
# optimization with debug info (for disassembly)
DEBUG?=0
OPT?=1
# do you want to show the commands executed ?
# Since we are using ?= for assignment it means that you can just
# set this from the command line and avoid changing the makefile...
DO_MKDBG?=0
# the c++ compiler to be used
CXX:=g++
CC:=gcc

#####################
# end of parameters #
#####################

# compilation flags
CXXFLAGS:=
CFLAGS:=
ifeq ($(DEBUG),1)
CXXFLAGS:=$(CXXFLAGS) -g3
CFLAGS:=$(CFLAGS) -g3
else
LDFLAGS:=$(LDFLAGS) -s
endif
ifeq ($(OPT),1)
CXXFLAGS:=$(CXXFLAGS) -O2
CFLAGS:=$(CFLAGS) -O2
endif
CXXFLAGS:=$(CXXFLAGS) -Wall -Werror -I$(US_INCLUDE)
CFLAGS:=$(CFLAGS) -Wall -Werror -I$(US_INCLUDE)

# dependency on the makefile itself
ifeq ($(DO_ALL_DEPS),1)
ALL_DEPS:=Makefile
else
ALL_DEPS:=
endif

# silent stuff
ifeq ($(DO_MKDBG),1)
Q:=
# we are not silent in this branch
else # DO_MKDBG
Q:=@
#.SILENT:
endif # DO_MKDBG

# sources from the git perspective
GIT_SOURCES:=$(shell ./scripts/git_wrapper.sh ls-files)
ALL:=
CLEAN:=
CLEAN_DIRS:=

# user space applications (c and c++)
CC_SRC:=$(shell ./scripts/find_wrapper.sh $(US_DIRS) $(KERNEL_DIR) -name "*.cc")
C_SRC:=$(shell ./scripts/find_wrapper.sh $(US_DIRS) $(KERNEL_DIR) -name "*.c" -and -not -name "mod_*.c")
ALL_C:=$(shell ./scripts/find_wrapper.sh . -name "*.c")
ALL_CC:=$(shell ./scripts/find_wrapper.sh . -name "*.cc")
ALL_H:=$(shell ./scripts/find_wrapper.sh . -name "*.h")
ALL_HH:=$(shell ./scripts/find_wrapper.sh . -name "*.hh")
CC_ASX:=$(addsuffix .s,$(basename $(CC_SRC)))
C_ASX:=$(addsuffix .s,$(basename $(C_SRC)))
CC_PRE:=$(addsuffix .p,$(basename $(CC_SRC)))
C_PRE:=$(addsuffix .p,$(basename $(C_SRC)))
CC_DIS:=$(addsuffix .dis,$(basename $(CC_SRC)))
C_DIS:=$(addsuffix .dis,$(basename $(C_SRC)))
CC_EXE:=$(addsuffix .exe,$(basename $(CC_SRC)))
C_EXE:=$(addsuffix .exe,$(basename $(C_SRC)))
ALL:=$(ALL) $(CC_EXE) $(C_EXE)
CLEAN:=$(CLEAN) $(CC_EXE) $(CC_DIS) $(CC_ASX) $(CC_PRE)

# kernel modules
MOD_SRC:=$(shell ./scripts/find_wrapper.sh $(KERNEL_DIR) -name "mod_*.c" -and -not -name "mod_*.mod.c")
MOD_BAS:=$(basename $(MOD_SRC))
MOD_OBJ:=$(addsuffix .o,$(MOD_BAS))
MOD_SR2:=$(addsuffix .mod.c,$(MOD_BAS))
MOD_OB2:=$(addsuffix .mod.o,$(MOD_BAS))
MOD_CM1:=$(addprefix $(KERNEL_DIR)/.,$(addsuffix .ko.cmd,$(notdir $(MOD_BAS))))
MOD_CM2:=$(addprefix $(KERNEL_DIR)/.,$(addsuffix .mod.o.cmd,$(notdir $(MOD_BAS))))
MOD_CM3:=$(addprefix $(KERNEL_DIR)/.,$(addsuffix .o.cmd,$(notdir $(MOD_BAS))))
MOD_MOD:=$(addsuffix .ko,$(MOD_BAS))
ALL:=$(ALL) $(MOD_MOD)
CLEAN:=$(CLEAN) $(MOD_MOD) $(MOD_SR2) $(MOD_OB2) $(KERNEL_DIR)/Module.symvers $(KERNEL_DIR)/modules.order $(MOD_CM1) $(MOD_CM2) $(MOD_CM3) $(MOD_OBJ)
CLEAN_DIRS:=$(CLEAN_DIRS) $(KERNEL_DIR)/.tmp_versions

# odps
ODP_SRC:=$(shell find doc/odp -name "*.odp")
ODP_BAS:=$(basename $(ODP_SRC))
ODP_PPT:=$(addsuffix .ppt,$(ODP_BAS))
ODP_PDF:=$(addsuffix .pdf,$(ODP_BAS))
ALL:=$(ALL) $(ODP_PPT) $(ODP_PDF)

# standlone
MK_SRC:=$(shell find src/examples_standalone -name "Makefile")
MK_FLD:=$(dir $(MK_SRC))
MK_STP:=$(addsuffix .stamp,$(MK_SRC))
ALL:=$(ALL) $(MK_STP)

# generic section
.PHONY: all
all: $(ALL)

.PHONY: ppt
ppt: $(ODP_PPT)

.PHONY: pdf
pdf: $(ODP_PDF)

.PHONY: soffice
soffice:
	$(Q)soffice "-accept=socket,port=2002;urp;" > /dev/null 2> /dev/null &

.PHONY: clean_manual
clean_manual:
	$(info doing [$@])
	$(Q)-rm -f $(CLEAN)
	$(Q)-rm -rf $(CLEAN_DIRS)

# -x: remove everything not known to git (not only ignore rules).
# -d: remove directories also.
# -f: force.
# I used to do:
# @git clean -xdf
# but it is too harsh
GIT_CLEAN_FLAGS=-xdf
#GIT_CLEAN_FLAGS=-fXd
.PHONY: clean
clean: clean_standalone
	$(info doing [$@])
	$(Q)git clean $(GIT_CLEAN_FLAGS) > /dev/null
.PHONY: clean_test
clean_test:
	@git clean $(GIT_CLEAN_FLAGS) --dry-run
.PHONY: clean_standalone
clean_standalone:
	$(Q)for x in $(MK_FLD); do $(MAKE) -C "$$x" clean Q=$(Q); if [ ! $$? -eq 0 ]; then exit $$?; fi; done
.PHONY: build_standalone
build_standalone:
	$(Q)for x in $(MK_FLD); do $(MAKE) -C "$$x" Q=$(Q); if [ ! $$? -eq 0 ]; then exit $$?; fi; done

# the reason that tar and gzip were selected and not zip is that the build system
# for the native demos requires scripts with permissions and stuff. This may be different
# for other languages where the permission stuff is not that important (e.g. php).
.PHONY: archive_src
archive_src:
	$(info doing [$@])
	$(Q)git archive --format=tar --prefix=archive_src/ HEAD src Makefile scripts | gzip > /tmp/archive_src.tar.gz
.PHONY: archive_ace
archive_ace:
	$(info doing [$@])
	$(Q)git archive --format=tar --prefix=archive_ace/ HEAD src/user_space/ace/examples | gzip > /tmp/ace.tar.gz

.PHONY: git_maintain
git_maintain:
	$(info doing [$@])
	$(Q)git gc

# general rules...

# how to create regular executables...
$(CC_EXE): %.exe: %.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CXX) $(CXXFLAGS) -o $@ $< $$EXTRA_FLAGS
$(C_EXE): %.exe: %.c $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CC) $(CFLAGS) -o $@ $< $$EXTRA_FLAGS
$(CC_ASX): %.s: %.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CXX) $(CXXFLAGS) -S -o $@ $< $$EXTRA_FLAGS
$(C_ASX): %.s: %.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CC) $(CFLAGS) -S -o $@ $< $$EXTRA_FLAGS
$(CC_PRE): %.p: %.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CXX) $(CXXFLAGS) -E -o $@ $< $$EXTRA_FLAGS
$(C_PRE): %.p: %.cc $(ALL_DEPS)
	$(info doing [$@])
	$(Q)./scripts/compile_wrapper.py $< $@ $(CC) $(CFLAGS) -E -o $@ $< $$EXTRA_FLAGS
$(CC_DIS) $(C_DIS): %.dis: %.exe $(ALL_DEPS)
	$(info doing [$@])
#	$(Q)objdump --demangle --disassemble --no-show-raw-insn --section=.text $< > $@
#	$(Q)objdump --demangle --source --disassemble --no-show-raw-insn --section=.text $< > $@
	$(Q)objdump --disassemble --source --demangle $< > $@

# rule about how to create .ko files...
$(MOD_MOD): %.ko: %.c $(ALL_DEPS) scripts/make_wrapper.pl
	$(info doing [$@])
	$(Q)scripts/make_wrapper.pl -C $(KDIR) V=$(V) KCFLAGS=$(KCFLAGS) M=$(abspath $(dir $<)) modules obj-m=$(addsuffix .o,$(notdir $(basename $<)))

# rules about odps
$(ODP_PPT): %.ppt: %.odp $(ALL_DEPS)
	$(info doing [$@])
	$(Q)rm -f $@
	$(Q)./scripts/DocumentConverter.py $< $@
	$(Q)chmod 444 $@
$(ODP_PDF): %.pdf: %.odp $(ALL_DEPS)
	$(info doing [$@])
	$(Q)rm -f $@
	$(Q)./scripts/DocumentConverter.py $< $@
	$(Q)chmod 444 $@

# rules about makefiles
$(MK_STP): %.stamp: % $(ALL_DEPS)
	$(info doing [$@])
	$(Q)$(MAKE) -C $(dir $<) Q=$(Q)
	$(Q)touch $@

# old junk for odps
#$(Q)java -jar ~/install/jodconverter-core-3.0-beta-4.jar $< $@
#$(Q)cp $< $@
#$(Q)java -jar ~/install/jodconverter-core-3.0-beta-4.jar $< $@
#$(Q)cp $< $@

.PHONY: debug
debug:
	$(info MOD_MOD is $(MOD_MOD))
	$(info CC_SRC is $(CC_SRC))
	$(info CC_DIS is $(CC_DIS))
	$(info CC_EXE is $(CC_EXE))
	$(info C_SRC is $(C_SRC))
	$(info C_DIS is $(C_DIS))
	$(info C_EXE is $(C_EXE))
	$(info MOD_SRC is $(MOD_SRC))
	$(info ALL is $(ALL))
	$(info KDIR is $(KDIR))
	$(info V is $(V))
	$(info KCFLAGS is $(KCFLAGS))
	$(info CLEAN is $(CLEAN))
	$(info CLEAN_DIRS is $(CLEAN_DIRS))
	$(info GIT_SOURCES is $(GIT_SOURCES))
	$(info ALL_DEPS is $(ALL_DEPS))
	$(info CXX is $(CXX))
	$(info CXXFLAGS is $(CXXFLAGS))
	$(info CC is $(CC))
	$(info CFLAGS is $(CFLAGS))
	$(info ODP_SRC is $(ODP_SRC))
	$(info ODP_PPT is $(ODP_PPT))
	$(info MK_SRC is $(MK_SRC))
	$(info MK_FLD is $(MK_FLD))
	$(info MK_STP is $(MK_STP))

.PHONY: todo
todo:
	-@grep TODO $(CC_SRC) $(C_SRC)

# various checks

.PHONY: check_ws
check_ws:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -l "\ \ " -- '*.h' '*.hh' '*.c' '*.cc'
	@./scripts/ok_wrapper.pl git grep -l " $$" -- '*.h' '*.hh' '*.c' '*.cc'
	@./scripts/ok_wrapper.pl git grep -l "\s$$" -- '*.h' '*.hh' '*.c' '*.cc'
	@./scripts/ok_wrapper.pl git grep -l "$$$$" -- '*.h' '*.hh' '*.c' '*.cc'
.PHONY: check_main
check_main:
	$(info doing [$@])
	-@git grep " main(" -- '*.h' '*.hh' '*.c' '*.cc' | grep -v argc
	-@git grep "ACE_TMAIN" -- '*.h' '*.hh' '*.c' '*.cc' | grep -v argc
.PHONY: check_ace_include
check_ace_include:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -l "include\"ace" -- '*.h' '*.hh' '*.c' '*.cc'
	@./scripts/ok_wrapper.pl git grep -l "include \"ace" -- '*.h' '*.hh' '*.c' '*.cc'
.PHONY: check_include
check_include:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -l "#include " -- '*.h' '*.hh' '*.c' '*.cc'
.PHONY: check_name
check_name:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -L "Mark Veltzer <mark.veltzer" -- '*.c' '*.cc' '*.h' '*.hh'
.PHONY: check_exit
check_exit:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -l "exit(1)" -- '*.c' '*.cc' '*.h' '*.hh'
.PHONY: check_pgrep
check_pgrep:
	$(info doing [$@])
	-@./scripts/grep.py "\\n\\n\\n| = |^ |\\t | \\t|\ \ |\\t\\n| \\n|= " "^.*\.cc$$|^.*\.hh$$|^.*\.c$$|^.*\.h$$" src | grep -v .mod.c
# " =" cannot be checked because of void foo(void* =0)
.PHONY: check_firstinclude
check_firstinclude:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -L "^#include<firstinclude.h>$$" -- '*.c' '*.cc' '*.h' '*.hh' | grep -v firstinclude | grep -v mod_ | grep -v shared.h | grep -v kernel_helper.h | grep -v kernel_standalone | grep -v examples_standalone
.PHONY: check_all
check_all: check_ws check_main check_ace_include check_include check_name check_exit check_pgrep check_firstinclude

.PHONY: check_dots
check_dots:
	$(info doing [$@])
	@./scripts/ok_wrapper.pl git grep -l " : " -- '*.h' '*.hh' '*.c' '*.cc'
# checks that dont pass
.PHONY: check_syn
check_syn:
	@./scripts/ok_wrapper.pl git grep -l "while (" -- '*.c' '*.h' '*.cc' '*.hh'
	@./scripts/ok_wrapper.pl git grep -l "for (" -- '*.c' '*.h' '*.cc' '*.hh'
	@./scripts/ok_wrapper.pl git grep -l "if (" -- '*.c' '*.h' '*.cc' '*.hh'
	@./scripts/ok_wrapper.pl git grep -l "switch (" -- '*.c' '*.h' '*.cc' '*.hh'

.PHONY: check_files
check_files:
	-find . -mindepth 2 -type f -and -not -name "*.cc" -and -not -name "*.h" -and -not -name "*.h" -and -not -name "*.txt" -and -not -name "*.conf" -and -not -name "*.ini" -and -not -name "*.sample" -and -not -name "*.data" -and -not -name "*.doc" -and -not -name "*.bash" -and -not -name "*.c"
.PHONY: check_tests_for_drivers
check_tests_for_drivers:
	cd $(KERNEL_DIR);for x in test_*.cc; do y=`echo $$x | cut -f 2- -d _`;z=mod_`basename $$y .cc`.c; if [ ! -f $$z ]; then echo "missing $$z"; fi ; done
	cd $(KERNEL_DIR);for x in mod_*.c; do y=`echo $$x | cut -f 2- -d _`;z=test_`basename $$y .c`.cc; if [ ! -f $$z ]; then echo "missing $$z"; fi ; done

# various file finds...

PROJECTS_EXPR:=-name ".project" -or -name ".cproject" -or -wholename "./nbproject/*"
SOURCE_EXPR:=-name "*.cc" -or -name "*.hh" -or -name "*.h" -or -name "*.c" -or -name "Makefile" -or -name "*.txt" -or -name "*.sed" -or -name "*.patch" -or -name "*.mk" -or -name "*.cfg" -or -name "*.sh" -or -name "*.cfg" -or -name "*.html" -or -name "*.css" -or -name "*.js" -or -name "*.ajax" -or -name "*.php" -or -name "*.gdb" -or -name ".gitignore" -or -name "*.pl" -or $(PROJECTS_EXPR) -or -name "*.gif" -or -name "*.png" -or -name "*.xml" -or -name "*.sxw" -or -name "*.sxg" -or -wholename "*/.settings/*" -or -name "*.doc" -or -name "*.pdf" -or -name "*.jar" -or -name ".classpath" -or -name "*.sqlite" -or -name "*.py"
TARGET_EXPR:=-name "*.exe" -or -name "*.d" -or -name "*.o" -or -name "*.so" -or -name "*.o.cmd" -or -name "*.ko" -or -name "*.ko.cmd" -or -wholename "*/.tmp_versions/*" -or -name "Module.symvers" -or -name "modules.order" -or -name "*.class" -or -name "*.stamp" -or -name "*.dis"
GIT_SOURCE_EXPR:=-type f $(addprefix -or -path ./,$(GIT_SOURCES))

.PHONY: find_not_source
find_not_source:
	-@find -type f -not -path "./.git/*" -and -not \( $(SOURCE_EXPR) \)
.PHONY: find_not_target
find_not_target:
	-@find -type f -not -path "./.git/*" -and -not \( $(TARGET_EXPR) \)
.PHONY: find_not_source_target
find_not_source_target:
	-@find -type f -not -path "./.git/*" -and -not \( $(SOURCE_EXPR) \) -and -not \( $(TARGET_EXPR) \)
.PHONY: find_not_git_target
find_not_git_target:
	-@find -type f -and -not \( $(GIT_SOURCE_EXPR) \) -and -not \( $(TARGET_EXPR) \)
.PHONY: find_not_git_source
find_not_git_source:
	-@find -type f -and -not \( $(GIT_SOURCE_EXPR) \)
.PHONY: find_exercises
find_exercises:
	-@find -type f -name "*_exercise.txt"

# kernel section

.PHONY: kern_clean
kern_clean:
	$(info doing [$@])
	$(Q)-rm -rf $(KERNEL_DIR)/.tmp_versions
	$(Q)-rm -f $(KERNEL_DIR)/Module.symvers $(KERNEL_DIR)/modules.order $(KERNEL_DIR)/mod_*.ko $(KERNEL_DIR)/mod_*.o $(KERNEL_DIR)/*.mod.c $(KERNEL_DIR)/.??*
.PHONY: kern_build
kern_build: $(MOD_MOD)
.PHONY: kern_help
kern_help:
	$(MAKE) -C $(KDIR) help
.PHONY: kern_tail
kern_tail:
	sudo tail /var/log/kern.log
.PHONY: kern_tailf
kern_tailf:
	sudo tail -f /var/log/kern.log
.PHONY: kern_syslog_tail
kern_syslog_tail:
	sudo tail /var/log/kern.log
.PHONY: kern_syslog_tailf
kern_syslog_tailf:
	sudo tail -f /var/log/kern.log
.PHONY: kern_dmesg
kern_dmesg:
	@sudo dmesg
.PHONY: kern_dmesg_clean
kern_dmesg_clean:
	@sudo dmesg -c > /dev/null
.PHONY: kern_halt
kern_halt:
	sudo halt
.PHONY: kern_reboot
kern_reboot:
	sudo reboot
.PHONY: kern_makeeasy
kern_makeeasy:
	sudo echo "%sudo ALL=NOPASSWD: ALL" >> /etc/sudoers

# code formatting

.PHONY: format_astyle
format_astyle: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)astyle --verbose --suffix=none --formatted --preserve-date --options=scripts/astyle.cfg $(ALL_C) $(ALL_CC) $(ALL_H) $(ALL_HH)
# I do not use uncrustify because it changes code that it already beautified...
.PHONY: format_uncrustify
format_uncrustify: $(ALL_DEPS)
	$(info doing [$@])
	#$(Q)uncrustify -c scripts/uncrustify.cfg --replace --no-backup $(ALL_C) $(ALL_CC) $(ALL_H) $(ALL_HH)
	$(Q)uncrustify -c scripts/uncrustify.cfg --no-backup $(ALL_C) $(ALL_CC) $(ALL_H) $(ALL_HH)
.PHONY: format_indent
format_indent: $(ALL_DEPS)
	$(info doing [$@])
	$(Q)indent $(ALL_C) $(ALL_CC) $(ALL_H) $(ALL_HH)

# code measurements

.PHONY: sloccount
sloccount:
	$(info doing [$@])
	$(Q)sloccount .
.PHONY: count_files
count_files:
	$(info doing [$@])
	$(Q)find . -name "*.cc" -or -name "*.c" | wc -l
