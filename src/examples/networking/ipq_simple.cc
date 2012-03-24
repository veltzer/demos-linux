/*
 * Example of a firewall in user space (man libipq).
 *
 *	Mark Veltzer
 *
 * In order to use this you must do:
 #!/bin/sh
 * sudo modprobe iptable_filter
 * sudo modprobe ip_queue
 * sudo iptables -A OUTPUT -j QUEUE
 * sudo ./ipq_simple
 *
 * EXTRA_LIBS=-lipq
 */

extern "C" {
#include <libipq.h>
}
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netfilter.h>
#include <stdio.h>
#include <stdlib.h> // for exit(3)
#include <string.h> // for strerror(3)

#define BUFSIZE 2048

static void die(struct ipq_handle *h) {
	ipq_perror("passer");
	ipq_destroy_handle(h);
	exit(1);
}

int main(int argc, char **argv, char **envp) {
	int status;
	unsigned char buf[BUFSIZE];
	struct ipq_handle *h;

	h = ipq_create_handle(0, NFPROTO_IPV4);
	//h = ipq_create_handle(0, NFPROTO_IPV6);
	if (!h) {
		die(h);
	}
	status = ipq_set_mode(h, IPQ_COPY_PACKET, BUFSIZE);
	if (status < 0) {
		die(h);
	}

	do {
		status = ipq_read(h, buf, BUFSIZE, 0);
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
	} while (1);
	ipq_destroy_handle(h);
	return(0);
}
