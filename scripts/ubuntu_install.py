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
short_release=release[:release.rfind('-')]
#print('release is',release)
#print('short_release is',short_release)

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
	'libboost1.49-dev', # for boost threading
	'libboost-thread1.49.0', # for the actual library
	'libpcap-dev',
	'libasound2-doc',
	'libasound2-dev',
	'libdmalloc-dev',
	'libcpufreq-dev', # for cpufreq.h
	'aspectc++',
	'libnetfilter-queue1',
	'libnetfilter-queue-dev',

	# kernel stuff
	'linux-headers-generic',
	'linux-headers-lowlatency',
	'linux-headers-'+short_release+'-generic',
	'linux-headers-'+short_release+'-lowlatency',

	# for systemtap - this one does not exist in the regular ubuntu archives
	# ubuntu does not always carry the "-lowlatency" dbgsym and that why
	# it is commented out...
	'linux-image-'+short_release+'-generic'+'-dbgsym',
	#'linux-image-'+short_release+'-lowlatency'+'-dbgsym',

	# tools
	'linux-tools-common', # for perf(1)
	'libreoffice-common', # for soffice conversion
	'python-uno', # for soffice conversion
	'electric-fence',
	'schedtool',
	'iotop', # for the iotop command
	'jnettop', # for the jnettop command
	'htop',
	'sysprof',
	'mutrace',
	'cpulimit', # for the cpulimit command
	'bridge-utils', # for bridging utilities to demo creation of a bridge
	'iptraf',
	'numactl',

	# package used tools
	'ccache', # for faster building
	'xutils-dev', # for makedepend(1)
	'uncrustify', # for code formatting example
	'indent', # for code formatting example
	'astyle', # for code formatting example

	# these packages are for working with linuxapi...:)
	'vim',
]

args=['sudo','apt-get','install']
args.extend(packs)
subprocess.check_call(args)
