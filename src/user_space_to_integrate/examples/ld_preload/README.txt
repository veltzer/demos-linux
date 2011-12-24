This example shows the use of LD_PRELOAD.

The local library libmylib.so overrides the default behaviour
of sin. Notice that the main executable does NOT link with this
library. We will order that linking to be made at runtime.

use as:
export LD_PRELOAD=$PWD/libmylib.so

or use linking:
ld -o main.new main libmylib.so
g++ -nostdlib -o main.new main libmylib.so
