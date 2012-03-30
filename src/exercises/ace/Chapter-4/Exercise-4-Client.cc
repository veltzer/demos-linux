#include<ace/INET_Addr.h>
#include<ace/SOCK_Stream.h>
#include<ace/SOCK_Connector.h>
#include<ace/Log_Msg.h>
#include<ace/OS_NS_unistd.h>
#include<ace/OS_NS_stdio.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int, ACE_TCHAR *[]) {
	ACE_INET_Addr addr;

	addr.set(20000, ACE_LOCALHOST);
	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;
	int bc;
	static char buf[64];
	ACE_Time_Value timeout(10, 0);

	int i = 0;
	if (connector.connect(peer, addr, &timeout) == -1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("Client connect")), 1);
	}
	while (true) {
		bc = peer.recv(buf, sizeof(buf));
		buf[bc] = 0;
		if (!bc) {
			printf("End\n");
			break;
		}
		peer.send_n("OK", 3, 0);
		printf("%2d. %s", ++i, buf);
		ACE_OS::sleep(1);
	}
	peer.close();
	return(0);
}
