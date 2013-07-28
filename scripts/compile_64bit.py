#!/usr/bin/python

"""
This script will ultimately:
- run a 64 bit virtual machine.
- wait till it's up
- pull all the sources on it.
- clean and compile all code.
- return the result and logs from that compilation.

Currently it just runs the virtual machine...
"""

from __future__ import print_function
import sys # for argv
import subprocess # for Popen

# this function is here because python2.6 does not have subprocess.check_output
def system_check_output(args):
	run_args=' '.join(args)
	pr=subprocess.Popen(run_args,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
	(output,errout)=pr.communicate()
	status=pr.returncode
	if output!='':
		print(output)
	if errout!='':
		print(errout)
	#sys.exit(status)
	sys.exit(0)

if len(sys.argv)!=1:
	raise ValueError('command line issue')

# run the command
system_check_output(['virtualbox','--startvm','ubuntu64bit'])
