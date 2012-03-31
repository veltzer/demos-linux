#!/bin/bash

# this is a script that wraps the activation of git. It is supposed to wrap activation
# of stuff like 'git ls-files'. It will not scream bloody murder if the current directory
# is not a git folder. DO NOT USE THIS AS A GENERAL GIT WRAPPER SINCE ERRORS ARE GOOD
# IF THIS IS NOT A GIT FOLDER.

if [[ -d .git ]]; then
	git $*
fi
