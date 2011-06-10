#!/bin/sh

# this script shows which optimizations are enabled by the various gcc flags:
gcc -c -Q -O --help=optimizers > O-opts.out
gcc -c -Q -O2 --help=optimizers > O2-opts.out
gcc -c -Q -O3 --help=optimizers > O3-opts.out
