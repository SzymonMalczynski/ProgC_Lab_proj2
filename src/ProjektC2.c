/*
 ============================================================================
 Name        : ProjektC2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
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
#include "udp.h"
#include "libs.h"




int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	//wejscie
	char input [32];
	int *count = malloc (sizeof (int));

	//Creating buffer that willl be my packet
	unsigned char * sendbuff;				//moze wskaznik?
	sendbuff=(unsigned char*)malloc(64); // increase in case of more data
	memset(sendbuff,0,64);







		//Constructing ipv6 header
		struct ip6_hdr *iphdr = (struct ip6_hdr *)(sendbuff + sizeof(struct ethhdr));
		createipv6header(sendbuff);



		//Constructing udp header
		struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct ip6_hdr) + sizeof(struct ethhdr));
		createudpheader(sendbuff);




		// UDP checksum
		uh->check=udp_sum_calc(&(uh->len),&(iphdr->ip6_src),&(iphdr->ip6_dst),sendbuff);



		printf ("Ile pakietow chcesz wyslac? : ");
		fgets (input, 32, stdin);
		*count=atoi(input);



		LoadToList(count,sendbuff);
		sendPacket(sendbuff);









	return EXIT_SUCCESS;
}
