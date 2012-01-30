//#include <ace/OS.h>
#include <ace/ARGV.h>
#include <ace/Get_Opt.h>
#include <ace/ARGV.h>
#include <iostream>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
using namespace std;
// Usage example: Time_Value_Class -e 36000 -i 1200
// Where: 1200 means 20 minutes
// 36000 means 10 hours

const ACE_Time_Value max_interval(60 * 60);  // 1 hour.
int main(int argc, char **argv) {
	//ACE_Time_Value expiration = ACE_OS::gettimeofday();
	ACE_Time_Value expiration;
	ACE_Time_Value interval;
	ACE_Time_Value current_time = expiration;

	ACE_Get_Opt opt(argc, argv, "e:i:");

	for (int c; (c = opt()) != -1;) {
		switch (c) {
		case 'e':
			// Arithmetic operation
			expiration += ACE_Time_Value(atoi(opt.opt_arg()));
			break;

		case 'i':
			// Assignment
			interval = ACE_Time_Value(atoi(opt.opt_arg()));
			break;
		}
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("interval=%d max_interval=%d\n"), interval.sec(), max_interval.sec()));
	if (interval > max_interval) {
		cout << "interval must be less than " << max_interval.sec() << endl;
	} else if (expiration > (ACE_Time_Value::max_time - interval)) {
		cout << "expiration + interval must be less than " << ACE_Time_Value::max_time.sec() << endl;
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("interval : %d seconds.\n"), interval.sec()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("expiration after: %d seconds.\n"), expiration.sec() - current_time.sec()));
	return(0);
}
