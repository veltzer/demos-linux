/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include <firstinclude.h>
#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2), splice(2), fcntl(2)
#include <unistd.h> // getpagesize(2), fcntl(2)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This demos shows how to use splice(2) to avoid copy to/from user space.
* The demo itself is just a simple version of cp(1)
*
* Notes:
* - we call splice(2) many times with size of 32768 which is 8*4096=8*pagesize
* This size was decided upon by stracing GNU coreutils cp and seeing what size
* it is using (it doesn't use zero-copy!).
*/

void copy_file(const char* filein, const char* fileout) {
	const size_t splice_size=getpagesize()*8;
	// lets create a pipe
	int pipe_fds[2];
	// lets make the pipe big!
	CHECK_NOT_M1(pipe(pipe_fds));
	//lets get the pipe size
	//size_t pipe_size=CHECK_NOT_M1(fcntl(pipe_fds[0],F_GETPIPE_SZ));
	//printf("pipe_size is %d\n",pipe_size);
	//lets set the pipe size
	CHECK_NOT_M1(fcntl(pipe_fds[0],F_SETPIPE_SZ,splice_size));
	int fdin=CHECK_NOT_M1(open(filein, O_RDONLY, 0666));
	int fdout=CHECK_NOT_M1(open(fileout, O_WRONLY|O_CREAT|O_TRUNC, 0666));
	// we need the return value outside the loop
	int ret;
	//this is the main copy loop
	//we go out of the loop because of error or eof
	//>0: would have kept us in the loop
	//=0: that is ok - it is end of file
	//-1: error
	while((ret=splice(fdin,0,pipe_fds[1],0,splice_size,SPLICE_F_MOVE))>0) {
		// TODO: handle short splices...
		CHECK_NOT_M1(splice(pipe_fds[0],0,fdout,0,ret,SPLICE_F_MOVE));
	}
	CHECK_NOT_M1(ret);
	CHECK_NOT_M1(close(pipe_fds[0]));
	CHECK_NOT_M1(close(pipe_fds[1]));
	CHECK_NOT_M1(close(fdin));
	CHECK_NOT_M1(close(fdout));
}

int main(int argc,char** argv,char** envp) {
	if(argc!=3) {
		fprintf(stderr,"usage: %s [infile] [outfile]\n",argv[0]);
		return EXIT_FAILURE;
	}
	const char* filein=argv[1];
	const char* fileout=argv[2];
	copy_file(filein,fileout);
	return EXIT_SUCCESS;
}
