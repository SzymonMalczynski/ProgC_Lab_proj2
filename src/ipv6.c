/*
 * ipv6.c
 *
 *  Created on: Apr 15, 2019
 *      Author: root
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/if_ether.h>
#include <netinet/ip6.h>
#include <linux/udp.h>
 #include <errno.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include "ipv6.h"

void createipv6header(unsigned char *sendbuff) {

	//Constructing ipv6 header
	struct ip6_hdr *iphdr = (struct ip6_hdr *)(sendbuff + sizeof(struct ethhdr));

		iphdr->ip6_ctlun.ip6_un1.ip6_un1_flow= htonl ((6 << 28) | (0 << 20) | 0);
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_hlim=255;
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_nxt=IPPROTO_UDP;

		int status;
		status=inet_pton (AF_INET6, "::1", &(iphdr->ip6_dst));
		status=inet_pton (AF_INET6, "::1", &(iphdr->ip6_src));



		int  total_len;

		total_len=sizeof(struct ethhdr);
		total_len+= sizeof(struct ip6_hdr);
		total_len+= sizeof(struct udphdr);

		iphdr->ip6_ctlun.ip6_un1.ip6_un1_plen = htons(total_len - sizeof(struct ethhdr));


}
