CFLAGS:=-Wall -Werror -I../../include

main.exe: main.c libfoo.so libbar.so
	$(Q)gcc $(CFLAGS) -o $@ main.c -L. -lbar -lfoo
libfoo.so: foo.c
	$(Q)gcc $(CFLAGS) -shared -o libfoo.so foo.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc
libbar.so: bar.c
	$(Q)gcc $(CFLAGS) -shared -o libbar.so bar.c -Xlinker -init -Xlinker my_init -Xlinker -fini -Xlinker my_fini -Xlinker --build-id=0xaabbcc

.PHONY: clean
clean:
	$(Q)-rm -f main.exe libfoo.so libbar.so
.PHONY: dump
dump:
	$(Q)objdump -t libfoo.so
.PHONY: dump_private
dump_private:
	$(Q)objdump -p libfoo.so
.PHONY: dump_all
dump_all:
	$(Q)objdump -s libfoo.so
