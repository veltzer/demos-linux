#include <log4cpp/Portability.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/SyslogAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/NDC.hh>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS

/*
 * EXTRA_LIBS=-llog4cpp
 *
 *			Mark Veltzer
 *
 */
int main(int argc, char** argv, char** envp) {
	log4cpp::SyslogAppender *syslogAppender = new log4cpp::SyslogAppender("syslog", "hellolog4cpp", LOG_USER);

	log4cpp::Category& root = log4cpp::Category::getRoot();

	root.setAppender(syslogAppender);

	root.error("root error");
	root.warn("root warn");

	return EXIT_SUCCESS;
}


//template char* std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_S_construct<char const*>(char const*, char const*, std::allocator<char> const&, std::forward_iterator_tag);
