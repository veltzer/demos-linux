#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cpufreq.h>

#include "us_helper.hh"

/*
 *      This is a demo which shows how, on i386 platforms, to read the rdtsc
 *      register in order to get a high precision timer. Mind you, you must
 *      be running on a machine that does not do frequency scaling in order
 *      for this to work...
 *
 *      TODO:
 *      - add reading of the cpu frequency.
 *      - add diffing the two values.
 *      - add measuring absolute time via the cpu frequency.
 *
 *      If you compare the results of this test with the cpu frequency information
 *      provided by the OS you will find a slight difference.
 *
 *      Where does this difference come from?
 *      The sleep(3) call which is called here is handled by the OS using the PIC
 *      interrupt handler clock while the RDTSC register that we call is from the
 *      CPU internal clock. This means that we are comparing two different clocks!
 *      In addition to all of this setting the fact that the CPU reports that it's
 *      frequency is 600 MHz doesn't means that it ticks: 600,000,000 (600 million
 *      times) per seconds exactly...
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=-lcpufreq
 */
void long_task(void) {
//	const int sleep_time=100;
//	usleep(sleep_time);
	sleep(1);

	/*
	 *      const int count=1000000;
	 *      for(int i=0;i<count;i++) {
	 *      }
	 */
}


int main(int argc, char **argv, char **envp) {
	printf("starting up...\n");
	int c;
	sc(syscall(__NR_getcpu, &c, NULL, NULL));
	printf("sched_getcpu() is %d\n", sched_getcpu());
	printf("getcpu(2) is %d\n", c);

	printf("starting long_task...\n");
	ticks_t start = getticks();
	long_task();
	ticks_t end = getticks();
	printf("finished...\n");
	ticks_t diff = end - start;
	printf("start RDTSC is %llu\n", start);
	printf("end RDTSC is %llu\n", end);
	printf("diff RDTSC is %llu\n", diff);
	printf("time expired in micros is %u\n", get_mic_diff(start, end));

	// get_clk_tck is no good at giviing you the cpu frequency
	// it gives a 100 which is supposed to be the kernel resched timer
	// but a linux system means very little
	// (Ingo does dynamic ticks anyway...)
	printf("get_clk_tck() is %d\n", get_clk_tck());
	printf("sysconf(_SC_CLK_TCK) is %ld\n", sysconf(_SC_CLK_TCK));

	// this one requires parsing
	const char *cmd = "cat /proc/cpuinfo | grep MH";
	printf("going to do command [%s]\n", cmd);
	sc(system(cmd));

	// this one requires root (sudo)
	const char *cmd2 = "sudo cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq";
	printf("going to do command [%s]\n", cmd2);
	sc(system(cmd2));

	// this one is ok for everyone
	const char *cmd3 = "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
	printf("going to do command [%s]\n", cmd3);
	sc(system(cmd3));

	// this is using libcpufreq (it actually does the about reading of the
	// file in /sys...)
	printf("cpufreq_get_freq_kernel(0) [%lu]\n", cpufreq_get_freq_kernel(0));
	// this is again using libcpufreq but will fail since it tries to access
	// the /sys file that requires root access)
	printf("cpufreq_get_freq_hardware(0) [%lu]\n", cpufreq_get_freq_hardware(0));
	// this is again using libcpufreq but will fail since it tries to access
	// the /proc file that requires root access)
	//printf("proc_get_freq_kernel(0) [%lu]\n",proc_get_freq_kernel(0));
	return(0);
}
