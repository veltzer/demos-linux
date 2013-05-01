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
	'libprocps0-dev',
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
	'iotop',
	'libpcap-dev',
	'libasound2-doc',
	'libasound2-dev',
	'numactl',
	'linux-headers-generic',
	#'linux-headers-generic-pae', # if you use a -pae kernel
	'linux-image-3.8.0-19-generic-dbgsym', # for systemtap
	'ccache', # for faster building
	'xutils-dev', # for makedepend(1)
]

args=['sudo','apt-get','install']
args.extend(packs)
subprocess.check_call(args)
