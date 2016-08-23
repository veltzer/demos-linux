#!/usr/bin/python3

'''
This demo shows the fact that removing a file does not stop the process accessing it from working
and even working correctly...
'''

import time # for sleep
import shutil # for copy

filename='/tmp/passwd'
shutil.copy('/etc/passwd', filename)
print('while I am working please remove [{0}] and see that I keep working right...'.format(filename))
f=open(filename, 'r')
while True:
    for line in f:
        print(line, end="")
        time.sleep(1)
    f.seek(0)
