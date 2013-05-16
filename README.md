The Linux API project
=====================
This project is a source code repository for instructors or expert programmers
who want to explore the Linux C/C++ API.
It has about 600 examples (as of 01/2013) I found are useful in explaining the Linux API.
The idea is to provide a good coverage of all major features and to resolve disputes
about exactly how a certain API works.

Topics covered by the examples
------------------------------
* Multi-threading
* Multi-processing
* Locking
* Calling system calls without C
* Performance
* Coding in assembly
* Various compiler directives
* Many more...

Platforms supported
-------------------
* i386
* ia64
Other platforms will be supported if someone is willing to do the work.
I usually work on intel 32 bit so I regularly check the code on ia32 and occasionaly
use a virtual machine to check that the code works on ia64.

Contributing
------------
This project needs help. fork me and commit.
Just open a github account, modify and add examples and ask me to commit...
A lot of the code is documented but some of it is not. More documentation would be welcome.
I would give attribution to whomever contributes.

License
-------
Code is licensed GPL3 and I hold the copyright unless explicity stolen as attributed in the source code.
I encourage people to use this source code in courses for instruction.
Please give me some credit for it if you do wind up using it and consider dropping
me a note about it so I could feel good...

Using it
--------
* you need python on your machine:
	try:
		`python --version`
	if python is missing then:
		for deb based distributions (debian, ubuntu, ...) do:
			`sudo apt-get install python`
		for rpm based distributions (fedora, redhat, centos,...) do:
			`sudo yum install python`
* clone the examples: `git clone git@github.com:veltzer/linuxapi.git`
* cd into it: `cd linuxapi`
* install the missing packages and headers needed to compile and run this project `./scripts/ubuntu_install.py`
* compile: `make`
* some of the examples may not compile for you because of missing libraries. Don't worry:
	you can build each specific example by issuing:
		`make src/examples/performance/cache_misser.elf`
	the name of the elf binary is the same as the example source code with .elf instead of
	.[c|cc].
	You must be at the root of the project to issue the 'make' command.
* the most important part: tweak the examples, try to prove me (my comments) wrong, have fun!

	Mark Veltzer <mark.veltzer@gmail.com>, 2011-2013
