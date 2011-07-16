main: main.c libfoo.so libbar.so
	gcc -o main main.c -L. -lbar -lfoo
libfoo.so: foo.c
	gcc -shared -o libfoo.so foo.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc
libbar.so: bar.c
	gcc -shared -o libbar.so bar.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc

.PHONY: clean dump dump_private dump_all
clean:
	-rm -f main libfoo.so libbar.so
dump:
	objdump -t libfoo.so
dump_private:
	objdump -p libfoo.so
dump_all:
	objdump -s libfoo.so
