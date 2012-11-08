# variables
CC:=icpc
# this is no optimization
#CFLAGS:=-O0 -I/usr/include/i386-linux-gnu -I../../include
# in the Intel compiler -O, -O2 or no flag for optimization are the same so it always optimizes
CFLAGS:=-restrict -I../../include
CFLAGS0:=-O0 $(CFLAGS)
CFLAGS1:=-O2 $(CFLAGS)
CFLAGS2:=-O2 -no-simd $(CFLAGS)
CFLAGS3:=-O2 -fargument-noalias $(CFLAGS)
CFLAGS4:=-O2 -opt-multi-version-aggressive $(CFLAGS)
# none of following flags makes the intel compiler actually show a difference between
# alias and non-alias...
CFLAGS5:=-no-simd -fargument-alias -no-opt-multi-version-aggressive -no-ansi-alias $(CFLAGS)

# targets
.PHONY: all
all: main0.exe main1.exe main2.exe main3.exe main4.exe main5.exe main6.exe main7.exe

main0.exe: main.cc
	$(CC) $(CFLAGS0) main.cc -o main0.exe
main1.exe: main.cc
	$(CC) $(CFLAGS1) main.cc -o main1.exe
main2.exe: main.cc
	$(CC) $(CFLAGS2) main.cc -o main2.exe
main3.exe: main.cc
	$(CC) $(CFLAGS3) main.cc -o main3.exe
main4.exe: main.cc
	$(CC) $(CFLAGS4) main.cc -o main4.exe
main5.exe: main.cc
	$(CC) $(CFLAGS5) main.cc -o main5.exe
main6.exe: main.cc
	g++ -mtune=native -O4 -I../../include main.cc -o main6.exe
main7.exe: main.cc
	g++ -I../../include main.cc -o main7.exe

.PHONY: clean
clean:
	-rm -f main0.exe main1.exe main2.exe main3.exe main4.exe main5.exe main6.exe main7.exe main.dis

.PHONY: dis
dis:
	objdump main1.exe --disassemble > main.dis
