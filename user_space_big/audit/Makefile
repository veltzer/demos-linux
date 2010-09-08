# on Intel making a shared library works without the -fpic but
# use it none the less, you can also add -nostdlib to avoid the
# default bahviour of linking with standard C/C++ runtime libraries.
FLAGS_FOR_SHARED_LIB:=-shared -fpic
# app name
APP:=main
# lib name
LIBNAME:=myaudit
# full lib name
FULLLIBNAME:=lib$(LIBNAME).so
# the compiler we will be using
CXX:=g++
# all targets
ALL:=$(APP) $(FULLLIBNAME)

# here come the targets

.PHONY: all
all: $(ALL)

$(FULLLIBNAME): $(LIBNAME).cc
	$(CXX) $(FLAGS_FOR_SHARED_LIB) -o $@ $<
$(APP): $(APP).cc
	$(CXX) -o $@ $<

.PHONY: clean
clean:
	-rm $(ALL)

.PHONY: show_results
show_results:
	ldd $(FULLLIBNAME) $(APP)
	ldd -v $(FULLLIBNAME)
	ldd -u $(FULLLIBNAME)

.PHONY: run_without_audit
run_without_audit:
	./$(APP)

.PHONY: run_with_audit
run_with_audit:
	LD_AUDIT=$(PWD)/$(FULLLIBNAME) ./$(APP)
