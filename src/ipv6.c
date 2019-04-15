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
	char input [32];
	printf ("Kreator nagłówka IPV6 \n");



	//Constructing ipv6 header
	struct ip6_hdr *iphdr = (struct ip6_hdr *)(sendbuff + sizeof(struct ethhdr));



	printf ("Podaj Hop Limit  : ");
	fgets (input, 32, stdin);
	iphdr->ip6_ctlun.ip6_un1.ip6_un1_hlim=atoi(input);

	printf ("Next header [0] UDP : ");
	fgets (input, 32, stdin);
	if (atoi(input)== 0)
	{
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_nxt=IPPROTO_UDP;
	}


		iphdr->ip6_ctlun.ip6_un1.ip6_un1_flow= htonl ((6 << 28) | (0 << 20) | 0);


		char *src,*dst;
		 src= malloc (INET6_ADDRSTRLEN);
	     dst= malloc (INET6_ADDRSTRLEN);


		printf ("Src Address: ");
			fgets (input, 32, stdin);
			memcpy ( src, input, strlen (input)-1 ); //funkcja fgets ma nakoncu stringa jeszcze znak
			int status;
			status=inet_pton (AF_INET6,src, &(iphdr->ip6_src));

		printf ("Dst Address: ");
		fgets (input, 32, stdin);
		memcpy ( dst, input, strlen (input)-1 ); //funkcja fgets ma nakoncu stringa jeszcze znak
		status=inet_pton (AF_INET6,dst, &(iphdr->ip6_dst));




		int  total_len;

		total_len=sizeof(struct ethhdr);
		total_len+= sizeof(struct ip6_hdr);
		total_len+= sizeof(struct udphdr);

		iphdr->ip6_ctlun.ip6_un1.ip6_un1_plen = htons(total_len - sizeof(struct ethhdr));


}
