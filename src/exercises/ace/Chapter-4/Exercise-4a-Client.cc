#include<ace/Task.h>
#include<ace/INET_Addr.h>
#include<ace/SOCK_Stream.h>
#include<ace/SOCK_Connector.h>
#include<ace/Log_Msg.h>
#include<ace/OS_NS_unistd.h>
#include<ace/OS_NS_stdio.h>

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */

class SocketHandler : public ACE_Task_Base {
public:
	void set_value(int ivalue) {
		value = ivalue;
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Setting value %d\n"), value));
	}


	virtual int svc(void) {
		ACE_INET_Addr addr;
		ACE_SOCK_Connector connector;
		ACE_SOCK_Stream peer;
		int bc;
		static char buf[64];

		ACE_Time_Value timeout(10, 0);

		int i = 0;

		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) SocketHandler Thread %d running\n"), value));
		addr.set(50000 + value, ACE_LOCALHOST);
		if (-1 == connector.connect(peer, addr, &timeout)) {
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("Client connect")), 1);
		}
		while (true) {
			bc = peer.recv(buf, sizeof(buf));
			peer.send_n("OK", 3, 0);
			buf[bc] = 0;
			if (!ACE_OS::strcmp(buf, "End")) {
				printf("Client: <%d> End\n", value);
				peer.close();
				return(0);
			}
			printf("Client: <%d> %2d. %s", value, ++i, buf);
			ACE_OS::sleep(1);
		}
		return(0);
	}


private:
	int value;
};

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	SocketHandler handler[3];

	for (int i = 0; i < 3; i++) {
		handler[i].set_value(i);
		handler[i].activate();
	}
	for (int i = 0; i < 3; i++) {
		handler[i].wait();
	}
	return(0);
}
