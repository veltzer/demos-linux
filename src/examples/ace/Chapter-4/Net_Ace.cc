#include<ace/INET_Addr.h>
#include<ace/SOCK_Stream.h>
#include<ace/SOCK_Connector.h>
#include<ace/Log_Msg.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * Mark Veltzer
 *
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int ACE_TMAIN(int argc,ACE_TCHAR** argv,ACE_TCHAR** envp) {
	ACE_INET_Addr srvr(50000, ACE_LOCALHOST);

	ACE_SOCK_Connector connector;
	ACE_SOCK_Stream peer;

	if (connector.connect(peer, srvr) == -1) {
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("connect")), 1);
	}
	char buf[64];
	peer.send_n("uptime\n", 7);
	int bc = peer.recv(buf, sizeof(buf));
	int res = write(1, buf, bc);
	res++;
	peer.close();
	return EXIT_SUCCESS;
}
