/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <sys/prctl.h>	// for prctl(2)
#include <string.h>	// for strncpy(3)
#include <sys/types.h>	// for getpid(2)
#include <unistd.h>	// for getpid(2)
#include <us_helper.h>	// for CHECK_ZERO(), TRACE()
#include <multi_processing.h>	// for my_system()

/*
 * This example shows how to use prctl to set/get the current process name.
 * The name itself is up to 16 bytes long.
 * It also shows that this is reflected in the output of tools like ps and in the /proc
 * filesystem.
 * Notice that the 'comm' attribute is what you are setting getting (/proc/[pid]/comm,
 * ps -o comm) and not other attributes like command, cmd and others from ps or from /proc.
 *
 * When would you want to use this?
 * - when you're doing lots of multi processing (fork(2)) and want to distinguish between
 * the various processes.
 * - when whatever it is that your doing depends on how you were run (envrionment,
 * directory, conditions that you find, time of day, argv[0], whatever...).
 * - set names of threads in a multi threaded application (will be in a separate demo).
 *
 * TODO:
 * - add a fork(2) to this demo and show that parent and child can have different names.
 */

void my_print_process_name() {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	TRACE("process name is [%s]", name);
}

void my_get_process_name(char* buffer, unsigned int bufsize) {
	const unsigned int size=16;
	char name[size];
	CHECK_ZERO(prctl(PR_GET_NAME, name));
	strncpy(buffer, name, bufsize);
}

void my_set_process_name(const char* newname) {
	const unsigned int size=16;
	char name[size];
	strncpy(name, newname, size);
	CHECK_ZERO(prctl(PR_SET_NAME, name));
}

void my_print_process_name_from_proc() {
	my_system("cat /proc/%d/comm", getpid());
}

int main(int argc, char** argv, char** envp) {
	TRACE("start");
	const char* newname="newpname";
	my_print_process_name();
	my_print_process_name_from_proc();
	char myname[256];
	my_get_process_name(myname, 256);
	my_system("ps -o comm | grep %s | grep -v grep", myname);
	my_set_process_name(newname);
	my_print_process_name();
	my_print_process_name_from_proc();
	my_system("ps -o comm | grep %s | grep -v grep", newname);
	TRACE("end");
	return EXIT_SUCCESS;
}
