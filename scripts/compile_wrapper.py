#!/usr/bin/python3

"""
This script wraps the running of the compiler with the right flags.
You must pass the compiler and the flags to it.
It will scan the source file in order to find specific flags
to be added to the compilation or linkage.
"""

import sys # for exit
import subprocess # for Popen
import codecs

def system_check_output(cmd, input=None, cwd=None, env=None): 
	pipe=subprocess.Popen(cmd, shell=True, cwd=cwd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
	(output,errout)=pipe.communicate(input=input) 
	status=pipe.returncode 
	if status:
		#raise ValueError('error in executing',cmd)
		sys.exit(status)
	return output.decode()

# parameters
debug=False

# start of code
script=sys.argv.pop(0)
showCmd=int(sys.argv.pop(0))
ccache=int(sys.argv.pop(0))
link=int(sys.argv.pop(0))
source=sys.argv.pop(0)
target=sys.argv.pop(0)
args=sys.argv
if debug:
	print('script is',script)
	print('source is',source)
	print('target is',target)
# scan the source code
for line in open(source):
	if link:
		f=line.find('EXTRA_LINK_CMDS=')
		if f!=-1:
			line.strip()
			f=line.find('EXTRA_LINK_CMDS=')+len('EXTRA_LINK_CMDS=')
			cmd=line[f:]
			cmd=cmd.replace('SOURCE',source)
			cmd=cmd.replace('TARGET',target)
			cmd=cmd.split()
			out=subprocess.check_output(cmd).decode()
			out=out.split()
			args.extend(out)
		f=line.find('EXTRA_LINK_FLAGS=')
		if f!=-1:
			line.strip()
			f=line.find('EXTRA_LINK_FLAGS=')+len('EXTRA_LINK_FLAGS=')
			cmd=line[f:]
			cmd=cmd.replace('SOURCE',source)
			cmd=cmd.replace('TARGET',target)
			cmd=cmd.split()
			args.extend(cmd)
	else:
		f=line.find('EXTRA_COMPILE_CMDS=')
		if f!=-1:
			line.strip()
			f=line.find('EXTRA_COMPILE_CMDS=')+len('EXTRA_COMPILE_CMDS=')
			cmd=line[f:]
			cmd=cmd.replace('SOURCE',source)
			cmd=cmd.replace('TARGET',target)
			out=system_check_output(cmd)
			if debug:
				print('out is',out)
			out=out.split()
			args.extend(out)
		f=line.find('EXTRA_COMPILE_FLAGS=')
		if f!=-1:
			line.strip()
			f=line.find('EXTRA_COMPILE_FLAGS=')+len('EXTRA_COMPILE_FLAGS=')
			cmd=line[f:]
			cmd=cmd.replace('SOURCE',source)
			cmd=cmd.replace('TARGET',target)
			args[1:1]=cmd.split()
		f=line.find('EXTRA_COMPILE_FLAGS_AFTER=')
		if f!=-1:
			line.strip()
			f=line.find('EXTRA_COMPILE_FLAGS_AFTER=')+len('EXTRA_COMPILE_FLAGS_AFTER=')
			cmd=line[f:]
			cmd=cmd.replace('SOURCE',source)
			cmd=cmd.replace('TARGET',target)
			args.extend(cmd.split())
if ccache and not link:
	args.insert(0,'ccache')
if showCmd:
	print('running',args)
try:
	subprocess.check_call(args)
except Exception as e:
	#print('e is',e,file=sys.stderr)
	#print(dir(e),file=sys.stderr)
	#print(e.returncode,file=sys.stderr)
	sys.exit(e.returncode)
