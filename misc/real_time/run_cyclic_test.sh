#!/bin/sh

taskset 2 cyclictest -m -t3 -n -p99 -i500 -d500
