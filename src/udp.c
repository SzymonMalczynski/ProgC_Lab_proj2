/*
 * udp.c
 *
 *  Created on: Apr 15, 2019
 *      Author: root
 */
#include "udp.h"
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

typedef unsigned short u16;
typedef unsigned long u32;

u16 udp_sum_calc(u16 len_udp, u16 src_addr[],u16 dest_addr[], u16 buff[])
{
u16 prot_udp=17;
u16 padd=0;
u16 word16;
u32 sum;
int padding=1;
int i;

	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	if (padding&1==1){
		padd=1;
		buff[len_udp]=0;
	}

	//initialize sum to zero
	sum=0;

	// make 16 bit words out of every two adjacent 8 bit words and
	// calculate the sum of all 16 vit words
	for (i=0;i<len_udp+padd;i=i+2){
		word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
		sum = sum + (unsigned long)word16;
	}
	// add the UDP pseudo header which contains the IP source and destinationn addresses
	for (i=0;i<4;i=i+2){
		word16 =((src_addr[i]<<8)&0xFF00)+(src_addr[i+1]&0xFF);
		sum=sum+word16;
	}
	for (i=0;i<4;i=i+2){
		word16 =((dest_addr[i]<<8)&0xFF00)+(dest_addr[i+1]&0xFF);
		sum=sum+word16;
	}
	// the protocol number and the length of the UDP packet
	sum = sum + prot_udp + len_udp;

	// keep only the last 16 bits of the 32 bit calculated sum and add the carries
    	while (sum>>16)
		sum = (sum & 0xFFFF)+(sum >> 16);

	// Take the one's complement of sum
	sum = ~sum;

return ((u16) sum);
}




void createudpheader(unsigned char *sendbuff) {
	char input [32];
	struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct ip6_hdr) + sizeof(struct ethhdr));

	printf ("Kreator nagłówka UDP \n");



	printf ("Podaj port źródłowy  : ");
	fgets (input, 32, stdin);
	uh->source = htons(atoi(input));


	printf ("Podaj port docelowy  : ");
	fgets (input, 32, stdin);
	uh->dest = htons(atoi(input));


	printf ("Pole udp checksum i lenght liczone automatycznie nacisnij 1 aby kontynuować   : ");
	fgets (input, 32, stdin);
		if (atoi(input)!= 1)
		{exit (EXIT_FAILURE);}



	uh->check = 0;


	int  total_len;

			total_len=sizeof(struct ethhdr);
			total_len+= sizeof(struct ip6_hdr);
			total_len+= sizeof(struct udphdr);
	uh->len = htons((total_len - sizeof(struct ip6_hdr) - sizeof(struct ethhdr)));




}












