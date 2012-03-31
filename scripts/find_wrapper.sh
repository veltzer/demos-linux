#!/bin/bash

# this is a script which wraps the activation of find. The only difference
# is that it does not scream bloody murder if the directory he is asked
# to search in (first argument) is non existant. In that case he simply says
# nothing (read as returns an empty file set). In case the directory does
# exist it simply calls find with the rest of the arguments.

if [[ -d $1 ]]; then
	find $*
fi
