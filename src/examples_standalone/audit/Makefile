include ../../../Makefile.defs

# on Intel making a shared library works without the -fpic but
# use it none the less, you can also add -nostdlib to avoid the
# default bahviour of linking with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared -fpic
# app name
APP:=app.$(SUFFIX_BIN)
# lib name
LIBNAME:=myaudit
# full lib name
FULLLIBNAME:=lib$(LIBNAME).so
# the compiler we will be using
CXX:=g++
# flags for the compiler
CXXFLAGS:=-Wall -Werror -O2 -I../../include
# all targets
ALL:=$(APP) $(FULLLIBNAME)

# here come the targets

.PHONY: all
all: $(ALL)

$(FULLLIBNAME): $(LIBNAME).cc
	$(Q)$(CXX) $(CXXFLAGS) $(FLAGS_FOR_SHARED_LIB) -o $@ $<
$(APP): $(basename $(APP)).cc
	$(Q)$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	$(Q)-rm -f $(ALL)

.PHONY: show_results
show_results:
	$(Q)ldd $(FULLLIBNAME) $(APP)
	$(Q)ldd -v $(FULLLIBNAME)
	$(Q)ldd -u $(FULLLIBNAME)

.PHONY: run_without_audit
run_without_audit:
	$(Q)./$(APP)

.PHONY: run_with_audit
run_with_audit:
	LD_AUDIT=$(PWD)/$(FULLLIBNAME) ./$(APP)
