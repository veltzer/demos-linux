#!/usr/bin/python

"""
Implemting grep in python in less than 10 lines of code...

	Mark Veltzer <mark@veltzer.net>
"""
import re # for .compile, .finditer
import sys # for argv, exit
import os # for .path.join

# command line usage...
if len(sys.argv)!=4:
	print("usage: grep.py [expr] [fileregexp] [folder]")
	sys.exit(1)
# first compile the regular expression to search for...
c=re.compile(sys.argv[1])
cf=re.compile(sys.argv[2])
folder=sys.argv[3]
debug=False
#debug=True

for root,dirs,files in os.walk(folder):
	for file in files:
		full=os.path.join(root,file)
		#if debug:
		#	print("file is %s" % full)
		if cf.match(full):
			if debug:
				print("doing file %s" % full)
			for num,line in enumerate(open(full)):
				for x in c.finditer(line):
					print("%s, %s: %s" % (full,num,line[:-1]))
