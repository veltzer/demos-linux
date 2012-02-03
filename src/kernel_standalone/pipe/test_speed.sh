#!/bin/bash

# this will test the speed of my pipe device

fuser -k /dev/pipe0
cat /dev/zero > /dev/pipe0 &
#dd if=/dev/zero of=/dev/pipe0 &
pipemeter < /dev/pipe0
fuser -k /dev/pipe0
