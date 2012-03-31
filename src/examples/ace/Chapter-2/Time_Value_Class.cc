#include<ace/ARGV.h>
#include<ace/Get_Opt.h>
#include<ace/ARGV.h>
#include<iostream>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Usage example: Time_Value_Class -e 36000 -i 1200
 * Where: 1200 means 20 minutes
 * 36000 means 10 hours
 *
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

int main(int argc, char **argv) {
	const ACE_Time_Value max_interval(60*60); // 1 hour.
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
		std::cout << "interval must be less than " << max_interval.sec() << std::endl;
	} else if (expiration > (ACE_Time_Value::max_time - interval)) {
		std::cout << "expiration + interval must be less than " << ACE_Time_Value::max_time.sec() << std::endl;
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("interval : %d seconds.\n"), interval.sec()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("expiration after: %d seconds.\n"), expiration.sec() - current_time.sec()));
	return EXIT_SUCCESS;
}
