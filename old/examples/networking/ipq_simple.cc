/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <libnetfilter_queue/libipq.h>
// libipq is not well adjusted for C++ in older ubuntu
// extern "C" {
// #include <libipq.h>
// }
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netfilter.h>
#include <stdio.h>	// for fprintf(3)
#include <stdlib.h>	// for EXIT_SUCCESS, exit(3), EXIT_FAILURE
#include <string.h>	// for strerror(3)
#include <err_utils.h>	// for CHECK_ZERO()

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
 * EXTRA_LINK_FLAGS=-lipq
 */

int main(int argc, char** argv, char** envp) {
	int status;
	unsigned char buf[2048];
	struct ipq_handle *h;

	h=CHECK_ZERO(ipq_create_handle(0, NFPROTO_IPV4));
	// h=CHECK_ZERO(ipq_create_handle(0, NFPROTO_IPV6));
	CHECK_GEZERO(ipq_set_mode(h, IPQ_COPY_PACKET, sizeof(buf)));
	do {
		CHECK_GEZERO(ipq_read(h, buf, sizeof(buf), 0));
		fprintf(stderr, "after read\n");
		switch (ipq_message_type(buf)) {
		case NLMSG_ERROR:
			fprintf(stderr, "Received error message %s\n",
				strerror(ipq_get_msgerr(buf)));
			exit(EXIT_FAILURE);
			break;
		case IPQM_PACKET: {
			fprintf(stderr, "got packet\n");
			ipq_packet_msg_t *m=ipq_get_packet(buf);
			CHECK_GEZERO(ipq_set_verdict(h, m->packet_id, NF_ACCEPT, 0, NULL));
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
