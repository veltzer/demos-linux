#!/bin/bash

# This script tests the speed of the regular linux pipe...
# Just break this script with 'CTRL+C'.
 
cat /dev/zero | pipemeter > /dev/null
