include ../../../Makefile.defs
INDENT:=indent --use-tabs
INDENT:=indent `cat indent.cfg`
UNCRUSTIFY:=uncrustify -c uncrustify.cfg
.PHONY: all
all: uncrustify indent

ALL_DEP:=uncrustify.cfg indent.cfg Makefile

.PHONY: indent
indent: file.indent.6.out file.indent.5.out file.indent.4.out file.indent.3.out file.indent.2.out file.indent.1.out file.cc $(ALL_DEP)

.PHONY: indent_diff
indenf_diff: indent
	$(info doing [$@])
	$(Q)-diff file.indent.1.out file.cc
	$(Q)echo ================================
	$(Q)-diff file.indent.2.out file.indent.3.out
	$(Q)echo ================================
	$(Q)-diff file.indent.3.out file.indent.4.out
	$(Q)echo ================================
	$(Q)-diff file.indent.4.out file.indent.5.out
	$(Q)echo ================================
	$(Q)-diff file.indent.5.out file.indent.6.out
file.indent.6.out: file.indent.5.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@
file.indent.5.out: file.indent.4.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@
file.indent.4.out: file.indent.3.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@
file.indent.3.out: file.indent.2.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@
file.indent.2.out: file.indent.1.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@
file.indent.1.out: file.cc $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(INDENT) $< -o $@

.PHONY: uncrustify
uncrustify: file.uncrustify.6.out file.uncrustify.5.out file.uncrustify.4.out file.uncrustify.3.out file.uncrustify.2.out file.uncrustify.1.out file.cc $(ALL_DEP)

.PHONY: uncrustify_diff
uncrustify_diff: uncrustify
	$(info doing [$@])
	$(Q)-diff file.uncrustify.1.out file.cc
	$(Q)echo ================================
	$(Q)-diff file.uncrustify.2.out file.uncrustify.3.out
	$(Q)echo ================================
	$(Q)-diff file.uncrustify.3.out file.uncrustify.4.out
	$(Q)echo ================================
	$(Q)-diff file.uncrustify.4.out file.uncrustify.5.out
	$(Q)echo ================================
	$(Q)-diff file.uncrustify.5.out file.uncrustify.6.out

file.uncrustify.6.out: file.uncrustify.5.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null
file.uncrustify.5.out: file.uncrustify.4.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null
file.uncrustify.4.out: file.uncrustify.3.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null
file.uncrustify.3.out: file.uncrustify.2.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null
file.uncrustify.2.out: file.uncrustify.1.out $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null
file.uncrustify.1.out: file.cc $(ALL_DEP)
	$(info doing [$@])
	$(Q)$(UNCRUSTIFY) -f $< -o $@ 2> /dev/null

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f file.*.?.out
