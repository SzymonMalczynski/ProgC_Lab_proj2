#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 54321
#define MESSAGE "Hello"
#define SERVADDR "127.0.0.1"

int main(void)
{
  int sock;
  socklen_t clilen;
  struct sockaddr_in6 server_addr, client_addr;
  char buffer[1024];
  char addrbuf[INET6_ADDRSTRLEN];

  /* create a DGRAM (UDP) socket in the INET6 (IPv6) protocol */
  sock = socket(PF_INET6, SOCK_DGRAM, 0);

  if (sock < 0) {
    perror("creating socket");
    exit(1);
  }





  /* it is an INET address */
  server_addr.sin6_family = AF_INET6;

  server_addr.sin6_addr=in6addr_loopback;

  /* the port we are going to send to, in network byte order */
  server_addr.sin6_port = htons(PORT);
while(1){
  /* now send a datagram */
  if (sendto(sock, MESSAGE, sizeof(MESSAGE), 0,
             (struct sockaddr *)&server_addr,
	     sizeof(server_addr)) < 0) {
      perror("sendto failed");
      exit(4);
  }
}
  close(sock);

  return 0;
}
