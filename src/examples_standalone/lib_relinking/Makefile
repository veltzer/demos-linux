include ../../../Makefile.mk

LIB:=/usr/lib/libmpi.so.0

.PHONY: obj
obj:
	$(info doing [$@])
	$(Q)gcc -c -o file.o file.cc -I../../include

.PHONY: link
link:
	$(info doing [$@])
	$(Q)ld --shared -o out --static $(LIB) 

.PHONY: strip
strip:
	$(info doing [$@])
	$(Q)objcopy --verbose --strip-all $(LIB)

.PHONY: copy
copy:
	$(info doing [$@])
	$(Q)objcopy --verbose file.o out.$(SUFFIX_LIB)

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f file.o
