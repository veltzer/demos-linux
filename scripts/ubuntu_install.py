#!/usr/bin/python

# this script will install all the required packages that you need on
# ubuntu to compile and work with this package.

import subprocess

packs=[
	'libncurses5',
	'libncurses5-dev',
	'libncursesw5',
	'libncursesw5-dev',
	'ncurses-doc',
	'libproc-dev',
	'libgnomeui-dev',
	'libsigc++-dev',
	'libgnomeuimm-2.6-dev',
	'libpq-dev',
	'liblog4cpp5-dev',
	'binutils-dev',
	'libmysqlclient-dev',
	'libcwd',
	'libqt4-dev',
	'libwxgtk2.8-dev',
	'libmysql++-dev',
	'libsdl1.2-dev',
	'dialog',
	'libace-dev',
	'iptables-dev',
	'cpulimit', # for the cpulimit command
	'bridge-utils', # for bridging utilities to demo creation of a bridge
	'htop',
	'sysprof',
	'mutrace',
	'iptraf',
]

args=['sudo','apt-get','install']
args.extend(packs)
subprocess.check_call(args)
