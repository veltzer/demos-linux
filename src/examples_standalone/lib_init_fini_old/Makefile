CFLAGS:=-Wall -Werror -I../../include

main.exe: main.c libfoo.so libbar.so
	gcc $(CFLAGS) -o $@ main.c -L. -lbar -lfoo
libfoo.so: foo.c
	gcc $(CFLAGS) -shared -o libfoo.so foo.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc
libbar.so: bar.c
	gcc $(CFLAGS) -shared -o libbar.so bar.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc

.PHONY: clean
clean:
	-rm -f main.exe libfoo.so libbar.so
.PHONY: dump
dump:
	objdump -t libfoo.so
.PHONY: dump_private
dump_private:
	objdump -p libfoo.so
.PHONY: dump_all
dump_all:
	objdump -s libfoo.so
