#!/usr/bin/python

import sys
import subprocess

def system_check_output(cmd, input=None, cwd=None, env=None): 
	pipe=subprocess.Popen(cmd, shell=True, cwd=cwd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
	(output,errout)=pipe.communicate(input=input) 
	status=pipe.returncode 
	if status:
		raise ValueError('error in executing',cmd)
	return output

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
		cmd=cmd.replace('SOURCE',source)
		cmd=cmd.replace('TARGET',target)
		cmd=cmd.split()
		out=subprocess.check_output(cmd)
		out=out.split()
		args.extend(out)
	f=line.find('EXTRA_LIBS=')
	if f!=-1:
		line.strip()
		f=line.find('EXTRA_LIBS=')+len('EXTRA_LIBS=')
		cmd=line[f:]
		cmd=cmd.replace('SOURCE',source)
		cmd=cmd.replace('TARGET',target)
		cmd=cmd.split()
		args.extend(cmd)
	f=line.find('EXTRA_SYSTEM=')
	if f!=-1:
		line.strip()
		f=line.find('EXTRA_SYSTEM=')+len('EXTRA_SYSTEM=')
		cmd=line[f:]
		cmd=cmd.replace('SOURCE',source)
		cmd=cmd.replace('TARGET',target)
		out=system_check_output(cmd)
		out=out.split()
		args.extend(out)
if debug:
	print 'running',args
subprocess.check_call(args)
