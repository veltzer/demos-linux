This is a directory exploring system tap.

How to install systemtap on ubuntu
==================================
first:
	apt-get install systemtap

then:
	one way:
		wget http://ddebs.ubuntu.com/pool/main/l/linux/linux-image-3.0.0-16-generic-dbgsym_3.0.0-16.29_i386.ddeb
		install it using dpkg --install.

second way (better):
	cat > /etc/apt/sources.list.d/ddebs.list << EOF
		deb http://ddebs.ubuntu.com/ quantal main restricted universe multiverse
		deb http://ddebs.ubuntu.com/ quantal-updates main restricted universe multiverse
	EOF
	apt-key adv --keyserver keyserver.ubuntu.com --recv-keys ECDCAD72428D7C01
	apt-get update
	apt-get install linux-image-$(uname -r)-dbgsym

	* remark - the current ddebs archive DOES NOT carry a dbgsym kernel package for the -lowlatency
	kernels. You will have to use a -generic kernel to use systemtap.

References:
http://www.ibm.com/developerworks/linux/library/l-systemtap/index.html
http://sourceware.org/systemtap/wiki/SystemtapOnUbuntu
