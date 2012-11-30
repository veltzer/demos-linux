LIB:=/usr/lib/libmpi.so.0

.PHONY: obj
obj:
	$(Q)gcc -c -o file.o file.cc -I../../include

.PHONY: link
link:
	$(Q)ld --shared -o out --static $(LIB) 

.PHONY: strip
strip:
	$(Q)objcopy --verbose --strip-all $(LIB)

.PHONY: copy
copy:
	$(Q)objcopy --verbose file.o out

.PHONY: clean
clean:
	$(Q)-rm -f file.o
