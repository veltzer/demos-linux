Notes:
It turns out the plugin must be built with g++ since the support library for the
plugin has all the symbols mangled.

This example only works for g++/gcc 4.7 and 4.8. On 4.6 it fails.

references:
https://gcc.gnu.org/onlinedocs/gccint/Plugins-building.html#Plugins-building
