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
#include <dlfcn.h>

#include "ipv6.h"
#include "udp.h"
#include "libs.h"




int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	void *handle,*handle2;

	handle=dlopen("/home/student/eclipse-workspacenowe/ProjektC2/src/ipv6.so",RTLD_LAZY);
		if (!handle) {
		        printf("Blad ladowania biblioteki");
		        exit(EXIT_FAILURE);
		    }

	//wejscie
	char input [32];
	int *count = malloc (sizeof (int));


	//Creating buffer that willl be my packet
	unsigned char * sendbuff;				//moze wskaznik?
	sendbuff=(unsigned char*)malloc(64); // increase in case of more data
	memset(sendbuff,0,64);








		//Constructing ipv6 header
		struct ip6_hdr *iphdr = (struct ip6_hdr *)(sendbuff + sizeof(struct ethhdr));
		//wskaznik na funkcje
		void (*createipv6header)(unsigned char *sendbuff);
		createipv6header=dlsym(handle,"createipv6header");
		createipv6header(sendbuff);

		int a1;
		a1=dlclose(handle);
		if(a1!=0)
		{
			 printf("Blad zamkniecia biblioteki");
		}




		//Constructing udp header

		struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct ip6_hdr) + sizeof(struct ethhdr));

		handle2=dlopen("/home/student/eclipse-workspacenowe/ProjektC2/src/udp.so",RTLD_LAZY);
			if (!handle2) {
			        printf("Blad ladowania biblioteki");
			        exit(EXIT_FAILURE);
			    }

			void (*createudpheader)(unsigned char *sendbuff);
			createudpheader=dlsym(handle,"createudpheader");
			createudpheader(sendbuff);


			int a2;
					a2=dlclose(handle);
					if(a2!=0)
					{
						 printf("Blad zamkniecia biblioteki");
					}



		// UDP checksum
		uh->check=udp_sum_calc(&(uh->len),&(iphdr->ip6_src),&(iphdr->ip6_dst),sendbuff);



		printf ("Ile pakietow chcesz wyslac? : ");
		fgets (input, 32, stdin);
		*count=atoi(input);



		LoadToList(count,sendbuff);
		sendPacket(sendbuff);









	return EXIT_SUCCESS;
}
