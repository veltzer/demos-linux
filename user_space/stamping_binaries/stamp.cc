/*
 *	This example shows how to stamp binaries so that you will be able to know exactly who
 *	made them, when, on which machine, what version of software, from what source file and
 *	made what version was used.
 *	- to get the time stamp we use gcc's __DATE__ __TIME__ __FILE__ __LINE__ and related macros.
 *	- the demo also shows how to actually print the version from your program if you want to.
 *	- the demo shows how to stamp EACH compilation unit separately so that you will know what
 *	  version of each individual object file is.
 *	- the demo shows how to put stuff in the stamp whether they be strings, numbers or whatever.
 *	- the demo actually demonstrates that the information got to the object or exe file using
 *	  various techniques.
 *	- the demo also puts the md5 checksum of the source into the object file which could be used to perfectly ascertain from which exact source was this object produced.
 *
 *	If you want to see more macros that the compiler provides just run:
 *	cpp -dM  < /dev/null
 *
 *		Mark Veltzer
 *
 * EXTRA_CMDS=echo -DCHECKSUM=`md5sum SOURCE | cut -f 1 -d " "` -DHOST=`hostname` -DUSER=$USER -DSYSTEM=`uname -a | tr ' ' '-'`
 */

#include<stdio.h> // for snprintf(3), printf(3)
#include<time.h> // for strptime(3), mktime(3)
#include<sys/types.h> // for getpid(2)
#include<unistd.h> // for getpid(2)

#include"us_helper.hh"

#define STRING_VERSION "1.23.56"
#define NUMERIC_VERSION 1.23.56

// this macro does two things:
// - makes sure that the compiler does not issue "unused variable" or tried to make
// the static variable go away just because no one is using it.
// - make sure that our data goes into it's own section of the object code.
#define SECTION ".compile_info"
#define ATTR __attribute__((section(SECTION),used))

// the static allows us to use a compiled on tag for each file so you can put it in a common
// header and get stamping for all files in your project.
static const char* ATTR id_file="id_file=" __FILE__;
static const char* ATTR id_base_file="id_base_file=" __BASE_FILE__;
static const char* ATTR id_host="id_host=" __stringify(HOST);
static const char* ATTR id_user="id_user=" __stringify(USER);
static const char* ATTR id_system="id_system=" __stringify(SYSTEM);
static const char* ATTR id_date="id_date=" __DATE__;
static const char* ATTR id_time="id_time=" __TIME__;
static const char* ATTR id_timestamp="id_timestamp=" __TIMESTAMP__;
static const char* ATTR id_string_version="id_string_version=" STRING_VERSION;
static const char* ATTR id_numeric_version="id_numeric_version=" __stringify(NUMERIC_VERSION);
static const char* ATTR id_checksum="id_checksum=" __stringify(CHECKSUM);
static const char* ATTR id_version="id_version=" __VERSION__;

const char* script="cpp/user_space/stamping_binaries/stamp.gdb";

int main(int argc, char **argv, char **envp) {
	printf("date is %s\n", __DATE__);
	printf("time is %s\n", __TIME__);
	struct tm tm;
	scpe(strptime(__DATE__ " " __TIME__, "%b %d %Y %H:%M:%S", &tm), NULL);
	time_t t = mktime(&tm);
	printf("t is %lu\n", t);

	const unsigned int len=1024;
	char cmd[len];

	// lets try to run strings(1) on our own binary to see the data...
	snprintf(cmd,len,"strings %s | grep id_ | grep -v grep",argv[0]);
	printf("\nrunning [%s]\n",cmd);
	scie(system(cmd),"system");

	// lets try to see the actual symbols using objdump(1)...
	snprintf(cmd,len,"objdump -C --section=" SECTION " -x %s | grep id_",argv[0]);
	printf("\nrunning [%s]\n",cmd);
	scie(system(cmd),"system");

	// lets dump our own core
	snprintf(cmd,len,"gcore -o /tmp/core %d",getpid());
	printf("\nrunning [%s]\n",cmd);
	scie(system(cmd),"system");

	// lets run gdb to see if we can see our information from a debugging session
	snprintf(cmd,len,"gdb -c /tmp/core.%d -q %s -x %s",getpid(),argv[0],script);
	printf("\nrunning [%s]\n",cmd);
	scie(system(cmd),"system");
	return(0);
}
