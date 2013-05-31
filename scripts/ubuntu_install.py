#!/usr/bin/python3

# this script will install all the required packages that you need on
# ubuntu to compile and work with this package.

# TODO:
# - enable the dbgsym package after this script also knows how to add the
# apt repository to the machine...
# - the papi library is not supported by this procedue. have this procedue
# install the papi library for me (either as .deb from my own repository
# or from source).
# - the intel compiler auto-installation is not supported.

import subprocess
import os

release=os.uname().release
print('release is',release)

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

	# kernel stuff
	'linux-headers-generic',
	'linux-headers-'+release,
	'linux-image-'+release+'-dbgsym', # for systemtap - this one does not exist in the regular ubuntu archives
	#'linux-headers-generic-pae', # if you use a -pae kernel

	'linux-tools-common', # for perf(1)
	'ccache', # for faster building
	'xutils-dev', # for makedepend(1)
	'libcpufreq-dev', # for cpufreq.h
	'libboost1.49-dev', # for boost threading
	'libboost-thread1.49.0', # for the actual library
	'libreoffice-common', # for soffice conversion
	'python-uno', # for soffice conversion
	'uncrustify', # for code formatting example
	'indent', # for code formatting example
	'libdmalloc-dev',
	'electric-fence',

	# these packages are for working with linuxapi...:)
	'vim',
]

args=['sudo','apt-get','install']
args.extend(packs)
subprocess.check_call(args)
