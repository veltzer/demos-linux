#!/usr/bin/python

import sys
import subprocess

debug=False
script=sys.argv.pop(0)
source=sys.argv.pop(0)
target=sys.argv.pop(0)
args=sys.argv
if debug:
	print 'script is',script
	print 'source is',source
	print 'target is',target
# scan the source code
for line in open(source):
	f=line.find('EXTRA_CMDS=')
	if f!=-1:
		line.strip()
		f=line.find('EXTRA_CMDS=')+len('EXTRA_CMDS=')
		cmd=line[f:]
		cmd=cmd.split()
		out=subprocess.check_output(cmd)
		out=out.split()
		args.extend(out)
	f=line.find('EXTRA_LIBS=')
	if f!=-1:
		line.strip()
		f=line.find('EXTRA_LIBS=')+len('EXTRA_LIBS=')
		cmd=line[f:]
		cmd=cmd.split()
		args.extend(cmd)
if debug:
	print 'running',args
subprocess.check_call(args)
