#!/usr/bin/python3

'''
This script runs "git ls-files" and checks that no files
are symbolic links
'''

import subprocess # for check_output
import os.path # for islink

process = subprocess.Popen(['git','ls-files'], stdout=subprocess.PIPE)
for line in process.stdout:
    current=line.decode('utf-8').rstrip()
    if os.path.islink(current):
        raise ValueError('found a symlink (this is bad) '+current)
