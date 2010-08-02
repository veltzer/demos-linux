all: bar.o zoo.o ins.o

bar.o: bar.cc
	g++ -Wall -Werror -c -o bar.o bar.cc
zoo.o: zoo.cc
	g++ -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o zoo.o zoo.cc
ins.o: ins.cc
	g++ -fno-implicit-inline-templates -fno-implicit-templates -Wall -Werror -c -o ins.o ins.cc
nm:
	nm -C bar.o
	nm -C zoo.o
	nm -C ins.o
clean:
	-rm *.o
