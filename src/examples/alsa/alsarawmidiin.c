/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

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

#include<firstinclude.h>

/*
* EXTRA_LIBS=-lasound
*/
// Programmer: Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat May 9 22:03:40 PDT 2009
// Last Modified: Sat May 9 22:03:46 PDT 2009
// Filename: alsarawmidiin.c
// Syntax: C; ALSA 1.0
// $Smake: gcc -o %b %f -lasound
//
// Description:	Receive MIDI data from a synthesizer using the ALSA rawmidi
// interface. Reversed engineered from amidi.c (found in ALSA
// utils 1.0.19 program set).
//
// First double-check that you have the ALSA system installed on your computer
// by running this command-line command:
// cat /proc/asound/version
// Which should return something like:
// Advanced Linux Sound Architecture Driver Version 1.0.17.
// This example program should work if the version number (1.0.17 in this
// case) is "1".
//
// Online documentation notes:
//
// http://www.alsa-project.org/alsa-doc/alsa-lib/rawmidi.html
//
// Using SND_RAWMIDI_NONBLOCK flag for snd_rawmidi_open() or
// snd_rawmidi_open_lconf() instruct device driver to return the -EBUSY
// error when device is already occupied with another application. This
// flag also changes behaviour of snd_rawmidi_write() and snd_rawmidi_read()
// returning -EAGAIN when no more bytes can be processed.
//
// http://www.alsa-project.org/alsa-doc/alsa-lib/group___raw_midi.html
//
// int snd_rawmidi_open(snd_rawmidi_t** input, snd_rawmidi_t output,
// const char* name, int mode)
// intput ==returned input handle (NULL if not wanted)
// output ==returned output handle (NULL if not wanted)
// name ==ASCII identifier of the rawmidi handle, such as "hw:1,0,0"
// mode ==open mode (see mode descriptions above):
// SND_RAWMIDI_NONBLOCK, SND_RAWMIDI_APPEND, SND_RAWMIDI_SYNC
//
// int snd_rawmidi_close(snd_rawmidi_t* rawmidi)
// Close a deviced opended by snd_rawmidi_open(). Returns an negative
// error code if there was an error closing the device.
//
// int snd_rawmidi_write(snd_rawmidi_t* output, char* data, int datasize)
// output ==midi output pointer setup with snd_rawmidi_open().
// data ==array of bytes to send.
// datasize ==number of bytes in the data array to write to MIDI output.
//
// const char* snd_strerror(int errornum)
// errornum ==error number returned by an ALSA snd__* function.
// Returns a string explaining the error number.
//

#include<alsa/asoundlib.h> /* Interface to the ALSA system */
#include<stdlib.h> // for EXIT_SUCCESS

// function declarations:
void errormessage(const char *format, ...);

///////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
	int status;
	int mode =SND_RAWMIDI_SYNC;
	snd_rawmidi_t* midiin =NULL;
	const char* portname ="hw:1,0,0"; // see alsarawportlist.c example program
	if ((argc > 1) && (strncmp("hw:", argv[1], 3) ==0)) {
		portname =argv[1];
	}
	if ((status =snd_rawmidi_open(&midiin, NULL, portname, mode)) < 0) {
		errormessage("Problem opening MIDI input: %s", snd_strerror(status));
		exit(EXIT_FAILURE);
	}

	int maxcount =1000; // Exit after this many bytes have been received.
	int count =0; // Current count of bytes received.
	char buffer[1]; // Storage for input buffer received
	while (count < maxcount) {
		if ((status =snd_rawmidi_read(midiin, buffer, 1)) < 0) {
			errormessage("Problem reading MIDI input: %s", snd_strerror(status));
		}
		count++;
		if ((unsigned char)buffer[0] >=0x80) { // command byte: print in hex
			printf("0x%x ", (unsigned char)buffer[0]);
		} else {
			printf("%d ", (unsigned char)buffer[0]);
		}
		fflush(stdout);
		if (count % 20 ==0) { // print a newline to avoid line-wrapping
			printf("\n");
		}
	}
	snd_rawmidi_close(midiin);
	midiin=NULL; // snd_rawmidi_close() does not clear invalid pointer,
	return EXIT_SUCCESS; // so might be a good idea to erase it after closing.
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// error -- print error message
//

void errormessage(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	putc('\n', stderr);
}
