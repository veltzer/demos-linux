.PHONY: link
link:
	ld --shared -o out --static /usr/lib/libaudio.so.2

.PHONY: strip
strip:
	objcopy --verbose --strip-all /usr/lib/libaudio.so.2 out

.PHONY: obj
obj:
	gcc -c -o file.o file.cc

.PHONY: copy
copy:
	objcopy --verbose file.o out

.PHONY: clean
clean:
	-rm -f file.o
