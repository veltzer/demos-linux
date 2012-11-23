LIB:=/usr/lib/libmpi.so.0

.PHONY: obj
obj:
	gcc -c -o file.o file.cc -I../../include

.PHONY: link
link:
	ld --shared -o out --static $(LIB) 

.PHONY: strip
strip:
	objcopy --verbose --strip-all $(LIB)

.PHONY: copy
copy:
	objcopy --verbose file.o out

.PHONY: clean
clean:
	-rm -f file.o
