CC:=gcc
CFLAGS:=-Wall -Werror -O2
SOURCES_MYAPP:=$(shell find myapp -name "*.c")
SOURCES_MYLIB:=$(shell find mylib -name "*.c")
SOURCES:=$(SOURCES_MYAPP) $(SOURCES_MYLIB)
OBJECTS_MYAPP:=$(addsuffix .o,$(basename $(SOURCES_MYAPP)))
OBJECTS_MYLIB:=$(addsuffix .o,$(basename $(SOURCES_MYLIB)))
MYLIB:=libmylib.so
MYAPP:=app.elf

$(MYAPP): $(OBJECTS_MYAPP) $(MYLIB)
	gcc -o $@ $(OBJECTS_MYAPP) -L. -lmylib
$(MYLIB): $(OBJECTS_MYLIB)
	gcc -shared -o $@ $(OBJECTS_MYLIB)

.PHONY: clean
clean:
	rm -f $(OBJECTS_MYAPP) $(OBJECTS_MYLIB) $(MYLIB) $(MYAPP)

.PHONY: debug
debug:
	$(info SOURCES_MYAPP is $(SOURCES_MYAPP))
	$(info SOURCES_MYLIB is $(SOURCES_MYLIB))
	$(info OBJECTS_MYAPP is $(OBJECTS_MYAPP))
	$(info OBJECTS_MYLIB is $(OBJECTS_MYLIB))

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)
	rm -f Makefile.bak
# DO NOT DELETE

myapp/app.o: /usr/include/stdio.h /usr/include/features.h
myapp/app.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
myapp/app.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
myapp/app.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
myapp/app.o: /usr/include/libio.h /usr/include/_G_config.h
myapp/app.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
myapp/app.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
myapp/app.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
myapp/app.o: /usr/include/endian.h /usr/include/bits/endian.h
myapp/app.o: /usr/include/bits/byteswap.h /usr/include/sys/types.h
myapp/app.o: /usr/include/time.h /usr/include/sys/select.h
myapp/app.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
myapp/app.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
myapp/app.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
