This example explores the features of gcc precompiled headers.

In order to see the difference in performance between using precompiled
headers and not do:

$ rm -f main*.precompiled.o
and then:
$ time make

and same for:

$ rm -f main*.noprecomp.o
and then:
$ time make
