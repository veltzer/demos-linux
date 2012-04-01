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

#include<stdio.h> // for printf(3)
#include<time.h> // for nanosleep(2)

#include<us_helper.h>

/*
 *	This is an example showing how to sleep for very short periods of
 *	time and proves that it is indeed so...
 *
 *	As you can see there is a diff of about 100 micro between the time
 *	to sleep and the real time slept. This is MUCH less than the HZ
 *	value for linux (ranging from 100HZ to 1000HZ which yields 10000 micro
 *	to 1000 micro which are much bigger values.
 *
 *	In order to actually prove this there is still a need to split this
 *	program into two threads the higher priority of which runs this
 *	code and the lower keeping the CPU occupied all the time (by doing
 *	some calculation). According to expection the difference from the
 *	expected sleep time should remain about 100 micro.
 *
 *	Where does this 100 micro come from ? The answer lies in the accuracy
 *	of the interrupt handler, it's clock and the time to reschedule...
 *	The real time patch to the kernel can reduce this further...
 *
 *	Take note that nanosleep does not involve signals and so is 'signal
 *	safe' and on machines with a good rtc will use it. read more in the
 *	documentation.
 *
 * EXTRA_LIBS=-lcpufreq
 *
 * TODO:
 * - it seems that this example does not work well in Ubuntu since the
 *	cpu_freq function returns 0 as the frequency of the cpu. Find out why
 *	and fix the example.
 * - use usleep and sleep also and compare the results to those of nanosleep.
 */
int main(int argc,char** argv,char** envp) {
	const unsigned int repeats = 100;

	for(unsigned int i=100;i<2000;i+=100) {
		unsigned long sum=0;
		for(unsigned int j=0;j<repeats;j++) {
			ticks_t start = getticks();
			timespec t;
			t.tv_sec = 0;
			t.tv_nsec = 1000 * i;
			nanosleep(&t, NULL);
			ticks_t end = getticks();
			sum += get_mic_diff(start, end);
		}
		printf("time expired for usleep of %u micros is %lu diff is %lu\n", i, sum / repeats, sum / repeats - i);
	}
	return EXIT_SUCCESS;
}
