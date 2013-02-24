#!/bin/bash

for x in `find . -name "*.hh" -or -name "*.cc" -or -name "*.c" -or -name "*.h"`; do sed 's/Copyright (C) 2011, 2012/Copyright (C) 2011-2013/g' $x > $x.tmp; mv -f $x.tmp $x;done
