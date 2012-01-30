#include "us_helper.hh" // for micro_diff
#include <syslog.h> // for openlog(3), syslog(3), closelog(3)
#include <stdio.h> // for printf(3), fopen(3), fclose(3), fflush(3)
#include <sys/time.h> // for gettimeofday(2)
#include <assert.h> // for assert(3)
#include <pthread.h> // for pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock
#include <stdarg.h> // for va_list, va_start, va_end

/*
 * This example explores syslog speed as compared to writing to a simple file.
 *
 * Notes:
 * - every messsage that I'm sending to syslog or to file has a number in it. This
 *   has two reasons: the most important is for syslog to actually log the message
 *   since if syslog gets two identical messages it does not log the second. Instead
 *   it waits for sometime and then says that the previous message repeated so and so
 *   times. The second reason is to include the printf like formatting code in the
 *   measurements.
 * - the tested code runs in a high priority thread to make sure that we measure times
 *   correctly.
 * - the fwrite implementation is fast because it does buffering. Maybe you are ok with
 *   that (you may lose data if you crash) and in that case you can use it.
 *
 * Results:
 * - by default you will find that syslog is much much slower than write.
 *   Output for example from this program:
 *   =====================================
 *   doing 1000 syslogs
 *   time in micro of one syslog: 55.897000
 *   doing 1000 writes
 *   time in micro of one write: 0.330000
 *   =====================================
 *   This is because rsyslogd in ubuntu is synchroneous. Most syslogd implementations are
 *   like that.
 * 
 * TODO:
 * - add three more test cases: open(2), write(2), close(2) with
 *   	- standard flags.
 *   	- O_ASYNC
 *   	- O_SYNC
 * - add another test with syslog which writes to a sysfs file instead.
 * - add another test case of asynchroneous syslog (damn it! how do I configure that?!?).
 * - explain the results in the text above.
 * - do better stats (min, max, variance and more - max is the most important).
 *
 * 					Mark Veltzer
 * EXTRA_LIBS=-lpthread
 */

// this emulates an async implementation
//pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
const unsigned int buffer_size=1024;
char buffer[buffer_size];
inline void my_syslog(const char* fmt,...) {
	//pthread_mutex_lock(&fastmutex);
	va_list args;
	va_start(args, fmt);
	int ret=vsnprintf(buffer, buffer_size, fmt, args);
	va_end(args);
	memcpy(buffer,fmt,ret);
	//pthread_mutex_unlock(&fastmutex);
}
inline void my_syslog(const char* fmt,...) __attribute__((format(printf, 1, 2)));

void* func(void*) {
	print_scheduling_info();
	// the name of this app
	const char* myname="syslog_speed";
	// number of messages to measure
	const unsigned int number=10000;
	// timevals to store before and after time...
	struct timeval t1, t2;
	// name of the test currently running
	const char* test;

	test="standard syslog";
	openlog(myname, LOG_PID, LOG_USER);
	printf("doing %d syslogs\n",number);
	// start timing...
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0; i < number; i++) {
		syslog(LOG_ERR,"this is a message %d", i);
	}
	// end timing...
	gettimeofday(&t2, NULL);
	closelog();
	// print timing...
	printf("%s: %lf\n",test,micro_diff(&t1,&t2)/(double)number);
	// let io buffers be flushed...
	sleep(1);

	test="regular file operations (nonbuffreed, flushed, synchroneous)";
	FILE* f=fopen("/tmp/syslog_test","w+");
	assert(f!=NULL);
	printf("doing %d writes\n",number);
	// start timing...
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0; i < number; i++) {
		fprintf(f,"this is a message %d", i);
		fflush(f);
	}
	// end timing...
	gettimeofday(&t2, NULL);
	fclose(f);
	// print timing...
	printf("%s: %lf\n",test,micro_diff(&t1,&t2)/(double)number);
	// let io buffers be flushed...
	sleep(1);

	test="regular file operations (buffered, non flushed, non synchronized)";
	f=fopen("/tmp/syslog_test","w+");
	assert(f!=NULL);
	printf("doing %d writes\n",number);
	// start timing...
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0; i < number; i++) {
		fprintf(f,"this is a message %d", i);
	}
	// end timing...
	gettimeofday(&t2, NULL);
	fclose(f);
	// print timing...
	printf("%s: %lf\n",test,micro_diff(&t1,&t2)/(double)number);
	// let io buffers be flushed...
	sleep(1);


	// now lets measure how long it would take to memcpy...
	test="fastlog";
	printf("doing %d fastlogs\n",number);
	// start timing...
	gettimeofday(&t1, NULL);
	for (unsigned int i = 0; i < number; i++) {
		my_syslog("this is a message %d", i);
	}
	// end timing...
	gettimeofday(&t2, NULL);
	// print timing...
	printf("%s: %lf\n",test,micro_diff(&t1,&t2)/(double)number);
	// let io buffers be flushed...
	sleep(1);

	return NULL;
}

int main(int argc, char **argv, char **envp) {
	print_scheduling_consts();
	run_high_priority(func,NULL,90);
	return 0;
}
