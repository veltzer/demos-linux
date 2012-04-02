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

#include<firstinclude.h>
// libipq is not well adjusted for C++
extern "C" {
#include<libipq.h>
}
#include<linux/in.h>
#include<linux/in6.h>
#include<linux/netfilter.h>
#include<stdio.h>
#include<stdlib.h> // for exit(3), EXIT_SUCCESS, EXIT_FAILURE
#include<string.h> // for strerror(3)

/*
* Example of a firewall in user space (man libipq).
*
* In order to use this you must do:
* #!/bin/sh
* sudo modprobe iptable_filter
* sudo modprobe ip_queue
* sudo iptables -A OUTPUT -j QUEUE
* sudo ./ipq_simple
*
* EXTRA_LIBS=-lipq
*/

static void die(struct ipq_handle *h) {
	ipq_perror("passer");
	ipq_destroy_handle(h);
	exit(EXIT_FAILURE);
}

int main(int argc,char** argv,char** envp) {
	const int bufsize=2048;
	int status;
	unsigned char buf[bufsize];
	struct ipq_handle *h;

	h = ipq_create_handle(0, NFPROTO_IPV4);
	//h = ipq_create_handle(0, NFPROTO_IPV6);
	if (!h) {
		die(h);
	}
	status = ipq_set_mode(h, IPQ_COPY_PACKET, bufsize);
	if (status < 0) {
		die(h);
	}

	do {
		status = ipq_read(h, buf, bufsize, 0);
		fprintf(stderr, "after read\n");
		if (status < 0) {
			die(h);
		}
		switch (ipq_message_type(buf)) {
		case NLMSG_ERROR:
			die(h);
			//fprintf(stderr, "Received error message %s\n",
			//	strerror(ipq_get_msgerr(buf)));
			break;

		case IPQM_PACKET: {
			fprintf(stderr, "got packet\n");
			ipq_packet_msg_t *m = ipq_get_packet(buf);
			status = ipq_set_verdict(h, m->packet_id,
				NF_ACCEPT, 0, NULL);
			if (status < 0) {
				die(h);
			}
			break;
		}

		default:
			fprintf(stderr, "Unknown message type!\n");
			break;
		}
	} while(true);
	ipq_destroy_handle(h);
	return EXIT_SUCCESS;
}
