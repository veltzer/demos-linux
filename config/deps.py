""" os level dependencies """

import os
import sys
import platform

# debug this script?
opt_debug=False
# exit after debug?
opt_exit=False
# install kernels?
opt_do_kernel=False
# install debug packages for kernels?
opt_do_ddebs=False
# do we want to install compilers?
opt_do_compilers=True
# do we want kernel related tools?
opt_do_kernel_tools=False


release=os.uname().release
short_release=release[:release.rfind("-")]
source_release=short_release[:short_release.rfind("-")]
release_lowlatency=short_release
release_generic=short_release

if opt_debug:
    print(f"release is [{release}]")
    print(f"short_release is [{short_release}]")
    print(f"source_release is [{source_release}]")
    print(f"release_lowlatency is [{release_lowlatency}]")
    print(f"release_generic is [{release_generic}]")

if opt_exit:
    sys.exit(0)

add = []
ver_wxgtk = None
lib_wx = None
ver_papi = None
ver_event = None
ver_ncurses = None
ver_asound = None
ver_urcu = None
ver_cds = None
ver_elf = None
ver_dw = None
ver_asm = None
ver_openmpi = None
ver_unwind = None
ver_boost_short = None
ver_boost = None

desktop = platform.freedesktop_os_release()
VERSION_ID = desktop["VERSION_ID"]
if VERSION_ID == "22.04":
    ver_wxgtk = "3.0"
    lib_wx = f"libwxgtk{ver_wxgtk}-gtk3-dev"
    ver_papi = "6.0"
    ver_event = "2.1-7"
    ver_ncurses = "5"
    ver_asound = "2"
    ver_urcu = "8"
    ver_cds = "???"
    ver_elf = "1"
    ver_dw = "1"
    ver_asm = "1"
    ver_openmpi = "3"
    ver_unwind = "8"
    ver_boost_short = "1.74"
    ver_boost=ver_boost_short+".0"
    add.extend([
        "dialog",
    ])
if VERSION_ID == "22.10":
    ver_wxgtk = "3.0"
    lib_wx = f"libwxgtk{ver_wxgtk}-gtk3-dev"
    ver_papi = "6.0"
    ver_event = "2.1-7a"
    ver_ncurses = "5"
    ver_asound = "2"
    ver_urcu = "8"
    ver_cds = "???"
    ver_elf = "1"
    ver_dw = "1"
    ver_asm = "1"
    ver_openmpi = "3"
    ver_unwind = "8"
    ver_boost_short = "1.74"
    ver_boost=ver_boost_short+".0"
    add.extend([
        "dialog",
    ])
if VERSION_ID == "23.04":
    ver_wxgtk = "3.2"
    lib_wx = f"libwxgtk{ver_wxgtk}-dev"
    ver_papi = "7.0"
    ver_event = "2.1-7a"
    ver_ncurses = "5"
    ver_asound = "2"
    ver_urcu = "8"
    ver_cds = "???"
    ver_elf = "1"
    ver_dw = "1"
    ver_asm = "1"
    ver_openmpi = "3"
    ver_unwind = "8"
    ver_boost_short = "1.74"
    ver_boost=ver_boost_short+".0"
    add.extend([
        "dialog",
    ])
if VERSION_ID == "23.10":
    ver_wxgtk = "3.2"
    lib_wx = f"libwxgtk{ver_wxgtk}-dev"
    ver_papi = "7.0"
    ver_event = "2.1-7"
    ver_ncurses = "6"
    ver_asound = "2"
    ver_urcu = "8"
    ver_cds = "???"
    ver_elf = "1"
    ver_dw = "1"
    ver_asm = "1"
    ver_openmpi = "3"
    ver_unwind = "8"
    ver_boost_short = "1.74"
    ver_boost=ver_boost_short+".0"
    add.extend([
        "dialog",
    ])
if VERSION_ID == "24.04":
    ver_wxgtk = "3.2-1t"
    lib_wx = "libwxgtk3.2-dev"
    ver_papi = "7.1t64"
    ver_event = "2.1-7t64"
    ver_ncurses = "6"
    ver_asound = "2t64"
    ver_urcu = "8t64"
    ver_cds = "2.3.3t64"
    ver_elf = "1t64"
    ver_dw = "1t64"
    ver_asm = "1t64"
    ver_openmpi = "3t64"
    ver_unwind = "8"
    ver_boost_short = "1.83"
    ver_boost=ver_boost_short+".0"
    add.extend([
        "dialog",
    ])
if VERSION_ID == "24.10":
    ver_wxgtk = "3.2-1t"
    lib_wx = "libwxgtk3.2-dev"
    ver_papi = "7.1t64"
    ver_event = "2.1-7t64"
    ver_ncurses = "6"
    ver_asound = "2t64"
    ver_urcu = "8t64"
    ver_cds = "2.3.3t64"
    ver_elf = "1t64"
    ver_dw = "1t64"
    ver_asm = "1t64"
    ver_openmpi = "3t64"
    ver_unwind = "8"
    ver_boost_short = "1.83"
    ver_boost=ver_boost_short+".0"
    add.extend([
        # libdialog
        "libdialog15",
        "libdialog-dev",
    ])

# check that versions were assigned
assert ver_wxgtk is not None
assert lib_wx is not None
assert ver_papi is not None
assert ver_event is not None
assert ver_ncurses is not None
assert ver_asound is not None
assert ver_urcu is not None
assert ver_cds is not None
assert ver_elf is not None
assert ver_dw is not None
assert ver_asm is not None
assert ver_openmpi is not None
assert ver_unwind is not None
assert ver_boost is not None
assert ver_boost_short is not None

packages_kernels=[
]
packages_doc=[
    "ncurses-doc", # ncurses documentation
    "binutils-doc", # binutils documentation
    "libasound2-doc",
    #"libgnomeui-doc",
    "libsigc++-2.0-doc",
    "libgtkmm-2.4-doc",
    "libgtkmm-3.0-doc",
    "libstdc++-9-doc",
    "aspell-doc",
    # libc
    "glibc-doc",
    "glibc-doc-reference",
    "scons-doc",
    #"jlint-doc",
    "make-doc",
    "systemtap-doc",
    "postgresql-doc",
    "papi-examples", # PAPI example files and test programs
    "libx11-doc",
]

packages_tools=[
    # ruby stuff
    "ruby-bundler",
    "rbenv",
    # for spelling
    "aspell",
    # debugging
    "cgdb",
    "qtcreator",
    # code measurements
    "sloccount",
    "cloc",
    # collection of command line tools
    "bikeshed",
    # manual pages
    "manpages",
    "manpages-dev",
    "manpages-posix",
    "manpages-posix-dev",
    # tools for building
    "gnulib",
    # tool for converting documents from one format to another
    "unoconv",
    "cpufrequtils",
    "cpu-checker",
    "netperf",
    # tools
    "shellcheck", # for checking shell scripts
    "linux-tools-common", # perf(1)
    "python3-uno", # soffice conversion
    #"vnstat", # causes performance problems
    #"vnstati", # causes performance problems
    "wireshark-common",
    "wireshark",
    "ngrep",
    "iftop",
    "traceroute",
    "valgrind",
    "dwarves",
    "kerneltop",
    "tshark",
    "google-perftools",
    "pv",
    "splint",
    "patchelf",
    "schedtool",
    "blktrace",
    "fdutils", # floppy disk utilities ?!?
    "iotop", # the iotop command
    "jnettop", # the jnettop command
    "smartmontools",
    "gsmartcontrol",
    "lm-sensors",
    "inxi",
    "nmap",
    "cgroup-tools",
    # doesn"t exist on 22.04
    # "hddtemp",
    "htop",
    "btop",
    "glances",
    "sysprof",
    # "mutrace",
    "cpulimit", # the cpulimit command
    "bridge-utils", # bridging utilities to demo creation of a bridge
    "iptraf-ng",
    "numactl",
    "pstack",
    "x86info",
    "lsscsi",
    "chrpath",
    "latencytop",
    "devmem2",
    "elfutils", # manipulating elf files
    "pax-utils",
    # "paxctl",
    "execstack",
    "prelink",
    "cpuid",
    "sysstat",
    "nmon",
    "saidar",
    # "sysdig",
    "iperf",
    "trace-cmd",
    "kernelshark",
    "smem",
    "sysbench",
    "gnome-system-monitor",
    "xfce4-taskmanager",
    "mrtg",
    "isag",
    "sdparm",
    # "mytop",
    "cutils",
    "hlint",
    "dlint",
    "oprofile",
    "powertop",
    "rt-tests",
    "procinfo",
    "wavemon",

    # fun stuff
    "figlet",
    "sl",

    # tools used by this package
    "libreoffice-common", # soffice conversion
    "xutils-dev", # makedepend(1)
    "astyle", # indent(1) (code formatting)

    # these packages are for the developers pleasure...:)
    "vim",
    "tofrodos",
    "pipemeter",
    "blktool",
    "scons",
    "doxygen",
    "make",

    # memory testing tools
    "memtester",
    "memtest86+",

    # papi
    "papi-tools", # PAPI utilities

    # papi

    # kernel debugging
    "systemtap",
    "systemtap-common",
    "systemtap-runtime",

    # kexec and kernel crash debugging
    "crash",
    "kdump-tools",
    "kexec-tools",
    "linux-crashdump",
    "makedumpfile",

    # databases
    "postgresql-client",
    "postgresql-client-common",

    # watchdogs
    "rtkit",
    "watchdog",
    "supervisor",
    "daemontools",
    #"monit",

    # firewalls
    "ufw",
    "shorewall",

    # javascript
    "nodejs",
    "npm",

    # openmpi
    "openmpi-bin",

    # tracing
    "strace",
    "ltrace",
]

packages=[
    # packages needed for the build
    "ccache",
    "uncrustify", # uncrustify(1) (code formatting)
    "indent", # indent(1) (code formatting)
    "electric-fence",

    # dialog really has header files and that is why it is here
    "dialog",
    "gcc-9-plugin-dev",
    "binutils-dev",
    "libxtables-dev",
    "libevent-dev",
    f"libevent-{ver_event}",
    "libiberty-dev",
    f"libncurses{ver_ncurses}",
    "libncurses-dev",
    f"libncursesw{ver_ncurses}",
    # f"libncursesw{ver_ncurses}-dev",
    # libprocps is gone
    # "libprocps8",
    # "libprocps-dev",
    "libsystemd0",
    "libsystemd-dev",
    "libsigc++-2.0-0v5",
    "libsigc++-2.0-dev",
    "libgtkmm-2.4-dev",
    "libgtkmm-3.0-dev",
    "libpq-dev",
    "liblog4cpp5-dev",
    "libmysqlclient-dev",
    lib_wx,
    "libmysql++-dev",
    "libsdl1.2-dev",
    "libace-dev",
    f"libboost{ver_boost_short}-dev",
    f"libboost-thread{ver_boost}",
    f"libboost-system{ver_boost}",
    "libboost-all-dev",
    "libpcap-dev",
    # asound lib
    f"libasound{ver_asound}",
    "libasound2-dev",
    "libdmalloc5",
    "libdmalloc-dev",
    "libcpufreq-dev", # cpufreq.h
    # netfilter library
    "libnetfilter-queue1",
    "libnetfilter-queue-dev",
    # capability.h
    "libcap-dev",
    # rcu library
    f"liburcu{ver_urcu}",
    "liburcu-dev",
    f"libcds{ver_cds}",
    "libcds-dev",
    f"libunwind{ver_unwind}", # unwind library
    "libunwind-setjmp0", # unwind library
    "libunwind-dev", # unwind library
    "libunwind-setjmp0-dev", # unwind library
    f"libelf{ver_elf}", # reading elf files
    # dw lib
    f"libdw{ver_dw}",
    "libdw-dev",
    # asm lib
    f"libasm{ver_asm}",
    "libasm-dev",
    "libaspell-dev",
    "libacl1-dev",
    "libattr1-dev",
    "libdaemon-dev",
    "libsystemd0",
    "libpapi-dev",
    f"libpapi{ver_papi}",
    "libpopt-dev",
    "systemtap-sdt-dev",
    "liburing-dev",
    # "libffi7",
    "libffi8",
    "libffi-dev",
    f"libopenmpi{ver_openmpi}",
    "libopenmpi-dev",
    "liburing2",
    "liburing-dev",
    # qemu stuff
    "qemu-system-x86",
    "virt-manager",
    "virtinst",
    "libvirt-clients",
    "bridge-utils",
    "libvirt-daemon-system",
    # qt stuff
    # this is for qt6
    "qt6-base-dev",
    # this is for qt5
    "qtbase5-dev",
    # linters
    "cppcheck",
    "cpplint",
    "clang-tidy",
]
packages.extend(add)

if opt_do_kernel:
    packages.extend([
        "linux-headers-generic",
        "linux-headers-lowlatency",
        f"linux-headers-{release_generic}-generic",
        f"linux-headers-{release_lowlatency}-lowlatency",
    ])
    packages_kernels.extend([
        f"linux-image-{release_generic}-generic",
        f"linux-image-{release_lowlatency}-lowlatency",
    ])
    packages_tools.extend([
        "linux-tools-common",
        "linux-tools-generic",
        "linux-tools-lowlatency",
        f"linux-tools-{release_generic}-generic",
        f"linux-tools-{release_lowlatency}-lowlatency",
    ])
    packages_doc.extend([
        "linux-source",
        f"linux-source-{source_release}",
    ])

if opt_do_ddebs:
    packages_kernels.extend([
        # systemtap - this one does not exist in the regular ubuntu archives
        # ubuntu does not always carry the "-lowlatency" dbgsym and that why
        # it is commented out...
        # TODO: make this script add sources to the dbgsyms if need be
        f"linux-image-{release_generic}-generic-dbgsym",
        f"linux-image-{release_lowlatency}-lowlatency-dbgsym",
    ])

if opt_do_compilers:
    packages.extend([
        "gcc",
        "gcc-multilib",
        "clang",
    ])
    packages_doc.extend([
        "gcc-doc",
        "cpp-doc",
    ])

packages.extend(packages_tools)

if opt_do_kernel_tools:
    packages.extend([
        "device-tree-compiler",
        "util-linux",
        "util-linux-extra",
    ])
