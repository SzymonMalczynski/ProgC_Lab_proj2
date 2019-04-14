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
























int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

		//otwarcie socketa raw do konstrukcji i wysylania datagramow
	int sock_raw;
	sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
	if(sock_raw == -1)
	printf("error in socket");

	//Getting the index name of the interface
	struct ifreq ifreq_i;
	memset(&ifreq_i,0,sizeof(ifreq_i));
	strncpy(ifreq_i.ifr_name,"lo",IFNAMSIZ-1); //giving name of Interface

	if((ioctl(sock_raw,SIOCGIFINDEX,&ifreq_i))<0)
	printf("error in index ioctl reading");//getting Index Name

	printf("index=%d\n",ifreq_i.ifr_ifindex);

	//Getting the MAC ADDRESS OF the interface

	struct ifreq ifreq_c;
	memset(&ifreq_c,0,sizeof(ifreq_c));
	strncpy(ifreq_c.ifr_name,"lo",IFNAMSIZ-1);//giving name of Interface

	if((ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
	printf("error in SIOCGIFHWADDR ioctl reading");


	//Getting IP address of the interface

	struct ifreq ifreq_ip;
	memset(&ifreq_ip,0,sizeof(ifreq_ip));
	strncpy(ifreq_ip.ifr_name,"lo",IFNAMSIZ-1);//giving name of Interface
	if(ioctl(sock_raw,SIOCGIFADDR,&ifreq_ip)<0) //getting IP Address
	{
	printf("error in SIOCGIFADDR \n");
	}

	//Creating buffer that willl be my packet
	unsigned char * sendbuff;				//moze wskaznik?
	sendbuff=(unsigned char*)malloc(64); // increase in case of more data
	memset(sendbuff,0,64);


	//Constructing Ethernet header

	struct ethhdr *eth = (struct ethhdr *)(sendbuff);

	eth->h_source[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
	eth->h_source[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
	eth->h_source[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
	eth->h_source[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
	eth->h_source[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
	eth->h_source[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);

	/* filling destination mac. DESTMAC0 to DESTMAC5 are macro having octets of mac address. */
	eth->h_dest[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
	eth->h_dest[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
	eth->h_dest[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
	eth->h_dest[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
	eth->h_dest[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
	eth->h_dest[5] =(unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);

	eth->h_proto = htons(ETHERTYPE_IPV6); //means next header will be IP header

	/* end of ethernet header */
	int  total_len;

	total_len=sizeof(struct ethhdr);



	//Constructing ipv6 header
		struct ip6_hdr *iphdr = (struct ip6_hdr *)(sendbuff + sizeof(struct ethhdr));

		iphdr->ip6_ctlun.ip6_un1.ip6_un1_flow= htonl ((6 << 28) | (0 << 20) | 0);
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_hlim=255;
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_nxt=IPPROTO_UDP;

		int status;
		status=inet_pton (AF_INET6, "::1", &(iphdr->ip6_dst));
		status=inet_pton (AF_INET6, "::1", &(iphdr->ip6_src));


		total_len+= sizeof(struct ip6_hdr);

		//Constructing udp header


		struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct ip6_hdr) + sizeof(struct ethhdr));

		uh->source = htons(23451);
		uh->dest = htons(23452);
		uh->check = 0;

		total_len+= sizeof(struct udphdr);


		//Sending data

		/* sendbuff[total_len++] = 0xAA;
sendbuff[total_len++] = 0xBB;
sendbuff[total_len++] = 0xCC;
sendbuff[total_len++] = 0xDD;
sendbuff[total_len++] = 0xEE;
		 */


		uh->len = htons((total_len - sizeof(struct ip6_hdr) - sizeof(struct ethhdr)));
		//UDP length field
		iphdr->ip6_ctlun.ip6_un1.ip6_un1_plen = htons(total_len - sizeof(struct ethhdr));
		//IP length field
		// UDP checksum
		uh->check=udp_sum_calc(&(uh->len),&(iphdr->ip6_src),&(iphdr->ip6_dst),sendbuff);

        //MAC//Wysylanie pakeitu

		struct sockaddr_ll sadr_ll;
		sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
		sadr_ll.sll_halen = ETH_ALEN; // length of destination mac address
		sadr_ll.sll_addr[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
		sadr_ll.sll_addr[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
		sadr_ll.sll_addr[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
		sadr_ll.sll_addr[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
		sadr_ll.sll_addr[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
		sadr_ll.sll_addr[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);






		//Wysylanie pakeitu

		int send_len;

		while(1){

		send_len = sendto(sock_raw,sendbuff,64,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
		if(send_len<0)
		{
		printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
		return -1;
		}
		}






	return EXIT_SUCCESS;
}
