include ../../../Makefile.mk

arch=$(shell uname -m)

ifeq ($(arch),i686)
STATIC_LIBC:=/usr/lib/i386-linux-gnu/libc.a
STATIC_LIBM:=/usr/lib/i386-linux-gnu/libm.a
endif

ifeq ($(arch),x64_86)
STATIC_LIBC:=/usr/lib/x86_64-linux-gnu/libc.a
STATIC_LIBM:=/usr/lib/x86_64-linux-gnu/libm.a
endif

CXXFLAGS:=-I. -I../../include -O2 -Wall -Werror
ALL:=\
	static1.$(SUFFIX_BIN)\
	static2.$(SUFFIX_BIN)\
	static3.$(SUFFIX_BIN)\
	static4.$(SUFFIX_BIN)\
	dynamic.$(SUFFIX_BIN)

.PHONY: all
all: $(ALL)
	$(info doing [$@])

# this works, producing an executable where everything is static
# note that there is still IFUNC like selection of the most optimized
# version of a function going on INSIDE the executable at runtime.
static1.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ -static $(CXXFLAGS) $^ -o $@

# this does not work!
static2.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)-g++ $(CXXFLAGS) $^ -o $@ $(STATIC_LIBC)
	$(Q)touch $@

# produces same executable as the fully dynamic version
static3.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $^ -o $@ -Wl,-Bstatic -lm -lc -Wl,-Bdynamic

# produces same executable as the fully dynamic version
static4.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $^ -o $@ $(STATIC_LIBM)

dynamic.$(SUFFIX_BIN): main.cc
	$(info doing [$@])
	$(Q)g++ $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(ALL)
