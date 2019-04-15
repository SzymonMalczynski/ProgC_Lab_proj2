/*
 * libs.c
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
#include "libs.h"
struct Node *head = NULL;

void sendPacket ( unsigned char *sendbuff ){


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

					 while ( head != NULL ){

					send_len = sendto(sock_raw,sendbuff,64,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
					if(send_len<0)
					{
					printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
					}
					else{
						head = head -> next;

					}


					}

					 DeleteList ();

}




void LoadToList ( int *count, unsigned char *dtgr ){

	for ( int i = 0; i != *count; i++ )
		InsertTail ( dtgr );
}



struct Node *ReserveMem ( unsigned char *datagram ){

	struct Node *new_node;
	static int id = 1;

	new_node = malloc(sizeof (struct Node));


	if (new_node == NULL) {
		printf("Cannot create new node");
		return NULL;
	}

	new_node->id = id;
	id++;
	new_node-> datagram = datagram;
	new_node->next = NULL;
	new_node->prev = NULL;


	return new_node;
}

void InsertTail ( unsigned char *datagram ){

	struct Node *temp = head;
	struct Node *new_node = ReserveMem ( datagram );


	if (head == NULL){
		head = new_node;
		return;
	}

	while (temp->next != NULL) //dodanie pakietu na koniec listy
		temp = temp->next;

	//dodanie nowego wezla
	temp->next = new_node;
	new_node->prev = temp;
	new_node->next = NULL;
}

void DeleteList (){

	struct Node *temp = head;
	struct Node *del = NULL;

	while ( temp != NULL ){
		del = temp;
		temp = temp -> next;
		free (del);
	}
	head = NULL;

}





















