include ../../../Makefile.defs

CFLAGS:=-fpic -Wall -Werror -I../../include

.PHONY: all
all: libuse_debug.so libuse_release.so

libadd_debug.so: add.c
	gcc $(CFLAGS) -g3 add.c -shared -o $@
libadd_release.so: add.c
	gcc $(CFLAGS) -O2 add.c -shared -o $@
libuse_debug.so: use.c libadd_debug.so
	rm -f libadd.so
	ln -s libadd_debug.so libadd.so
	gcc $(CFLAGS) use.c -shared -o $@ -L. -ladd
libuse_release.so: use.c libadd_release.so
	rm -f libadd.so
	ln -s libadd_release.so libadd.so
	gcc $(CFLAGS) use.c -shared -o $@ -L. -ladd

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.so

.PHONY: cmp
cmp:
	cmp libuse_*
