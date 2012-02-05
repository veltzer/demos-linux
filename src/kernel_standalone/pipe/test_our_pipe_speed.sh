#!/bin/bash

# this will test the speed of my pipe device

fuser -k /dev/pipe0
dd if=/dev/zero bs=50000 count=1000000 of=/dev/pipe0 &
# this is to make sure that the writing starts before the reading.
sleep 1
#dd if=/dev/zero of=/dev/pipe0 &
pipemeter < /dev/pipe0 > /dev/null &
# wait for the two background processes to be over
wait
