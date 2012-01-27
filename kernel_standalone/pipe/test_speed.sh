#!/bin/bash

# this will test the speed of my pipe device

fuser -k /dev/mypipe0
cat /dev/zero > /dev/mypipe0 &
#dd if=/dev/zero of=/dev/mypipe0 &
pipemeter < /dev/mypipe0
fuser -k /dev/mypipe0
