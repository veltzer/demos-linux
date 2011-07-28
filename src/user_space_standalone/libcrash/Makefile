
# Add debug information and asserts? 
DEBUG_BUILD ?= y

# Build multi threaded version of the library?
USE_THREADS ?= y

# Location and prefix of cross compiler, Linux kernel style
CROSS_COMPILE ?=

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump


CFLAGS	:= -Wall 
LDFLAGS := 

LIBS_CFLAGS  := $(CFLAGS)
LIBS_LDFLAGS := $(LDLAGS) -lrt -shared

ifdef USE_THREADS
LIBS_CFLAGS	+= -pthread -D_REENTRANT -DUSE_THREADS
LIBS_LDFLAGS += -pthread
endif

ifdef DEBUG_BUILD
LIBS_CFLAGS	+= -g
else
LIBS_CFLAGS	+= -DNDEBUG
endif

TESTS_CFLAGS = $(CFLAGS) -g -pthread -D_REENTRANT
TESTS_LDFLAGS = $(LDFLAGS) -rdynamic -lrt

LIB_API_VERSION = 1
LIB_VERSION = 1.0.0

.PHONY: all clean libs tests dist

all: libs tests

# The regular and automagic version of the library

libs: libcrash.so.$(LIB_VERSION) libauto_crash.so.$(LIB_VERSION) 

libcrash.so.$(LIB_VERSION): crash.c crashd.c crash.h crash_msg.h Makefile
	$(CC) -fPIC crash.c crashd.c $(LIBS_CFLAGS) $(LIBS_LDFLAGS) \
		-Wl,-soname,libcrash.so.$(LIB_API_VERSION) -o libcrash.so.$(LIB_VERSION)

crash.m: crash.c
	$(CPP) $< -o $@

libauto_crash.so.$(LIB_VERSION): auto_crash.c libcrash.so.$(LIB_VERSION) crash.h Makefile
	$(CC) -fPIC auto_crash.c libcrash.so.$(LIB_VERSION) $(LIBS_CFLAGS) $(LIBS_LDFLAGS) \
		-Wl,-soname,libauto_crash.so.$(LIB_API_VERSION) -o libauto_crash.so.$(LIB_VERSION) 


ifdef USE_THREADS

# Unit tests.
# Make sure to ldconfig -n `pwd` before running

tests: test auto_test naive_test

test: libcrash.so.$(LIB_VERSION) test.c crash.c crashd.c crash.h crash_msg.h Makefile
	$(CC) test.c crash.c crashd.c $(TESTS_CFLAGS) $(TESTS_LDFLAGS) -o test 
	
auto_test: libauto_crash.so.$(LIB_VERSION) auto_test.c crash.h crash_msg.h Makefile
	$(CC) auto_test.c $(TESTS_CFLAGS) $(TESTS_LDFLAGS) -o auto_test
	
naive_test: naive.c Makefile
	$(CC) naive.c  $(TESTS_CFLAGS) $(TESTS_LDFLAGS) -o naive_test
	
else 
  
tests: 
	@echo Tests not built due to lack of thread support.
endif

dist: clean
	rm -f libcrash-$(LIB_VERSION).tar.bz2 && \
	rm -rf libcrash-$(LIB_VERSION) && \
	mkdir libcrash-$(LIB_VERSION) && \
	cp *.c *.h Makefile README libcrash-$(LIB_VERSION) && \
	tar jcvf libcrash-$(LIB_VERSION).tar.bz2 libcrash-$(LIB_VERSION) 
	rm -rf libcrash-$(LIB_VERSION)

clean: 
	rm -f *.o naive_test auto_test test *~ libcrash.so.$(LIB_VERSION) \
		libauto_crash.so.$(LIB_VERSION) 
