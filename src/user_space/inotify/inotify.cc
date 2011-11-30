#include <sys/inotify.h> // for inotify_init(2), inotify_add_watch(2)
#include <stdio.h> // for printf(3)
#include <unistd.h> // for read(2), close(2)
#include <assert.h> // for assert(3)
#include <errno.h> // for errno(3)
#include <sys/types.h> // for getpid(2)
#include <unistd.h> // for getpid(2)
#include <signal.h> // for siginterrupt(2)
#include <limits.h> // for PATH_MAX

#include "us_helper.hh"

/*
 * This demo shows how to use the inotify(2) API to get notifications of file changes.
 * We use a signal to break out of the inotify loop. We could have used poll or an event
 * fd from other threads which would work out just fine.
 *
 *              Mark Veltzer
 *
 * EXTRA_LIBS=
 */

// signal handling functions
static bool stop=false;
static void handler(int sig, siginfo_t *si, void *unused) {
	printf("got signal, you probably want me to stop so I'm stopping...\n");
	stop=true;
}
static void register_handler() {
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sc(sigaction(SIGUSR1, &sa, NULL));
}
// print an inotify mask in readable form
static uint32_t types[]={
	IN_ACCESS, IN_ATTRIB, IN_CLOSE_WRITE,
	IN_CLOSE_NOWRITE, IN_CREATE, IN_DELETE,
	IN_DELETE_SELF, IN_MODIFY, IN_MOVE_SELF,
	IN_MOVED_FROM, IN_MOVED_TO, IN_OPEN };
static const char* names[]={
	"IN_ACCESS", "IN_ATTRIB", "IN_CLOSE_WRITE",
	"IN_CLOSE_NOWRITE", "IN_CREATE", "IN_DELETE",
	"IN_DELETE_SELF", "IN_MODIFY", "IN_MOVE_SELF",
	"IN_MOVED_FROM", "IN_MOVED_TO", "IN_OPEN" };
static void print_mask(uint32_t mask) {
	bool first=true;
	for(unsigned int i=0;i<sizeof(types)/sizeof(uint32_t);i++) {
		uint32_t m=types[i];
		if(mask & m) {
			if(first)
				printf("%s",names[i]);
			else
				printf("-%s",names[i]);
		}
	}
}

int main(int argc, char **argv, char **envp) {
	int fd;
	const char* path="/tmp";
	register_handler();
	sc(siginterrupt(SIGUSR1,1));
	sc(fd=inotify_init());
	uint32_t mask=IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM;
	sc(inotify_add_watch(fd,path,mask));
	printf("fd is %d\n", fd);
	printf("stop me with [kill -s SIGUSR1 %d]\n",getpid());
	printf("trace me with [strace -p %d]\n",getpid());
	printf("watching [%s] - change files to see me respond...\n",path);
	while(!stop) {
		const int size=sizeof(inotify_event)+PATH_MAX+1;
		char* buf[size];
		int res=read(fd,buf,size);
		if(res==-1) {
			int err=errno;
			if(err!=EINTR) {
				sc(res);
			} else {
				continue;
			}
		}
		// return size must be AT LEAST sizeof(inotify_event)
		assert(res>=(int)sizeof(inotify_event));
		struct inotify_event* ie=(struct inotify_event*)buf;
		// print the event
		printf("event-> file %s, mask ",ie->name);
		print_mask(ie->mask);
		printf("\n");
	}
	sc(close(fd));
	return(0);
}
