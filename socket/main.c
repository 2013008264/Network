#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MY_PORT 3030
#define BACKLOG 10

int main() {
	int sockfd; // socket file descriptor.
	int receiveSockFd;
	struct sockaddr_in myAddr;
	struct sockaddr_in theirAddr;
	unsigned int sin_size;

	/**
	 * Make UDP Socket
	 * PF_INET : Packet format, Internet = TCP / IP or UDP / IP
	 * AF_INET : Exactly same with PF_INET
	 * PF_INET6 : Using IPv6 protocol.
	 * PF_LOCAL, AF_UNIX : Communication with same system.
	 * PF_PACKET : Using low level socket interface.
	 * SOCK_STREAM : Using TCP
	 * SOCK_DGRAM : Using UDP.
	 **/
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Failed to make socket");
		return 1;
	}

	myAddr.sin_family = AF_INET; // Address family of internet.
	// htons == host to network short
	myAddr.sin_port = htons(MY_PORT);
	// htonl == host to network long
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding socket addr.
	if(bind(sockfd, (struct sockaddr *)&myAddr, sizeof(struct sockaddr)) == -1) {
		perror("Failed to bind socket");
		return 1;
	}

	// Wait listen.
	if(listen(sockfd, BACKLOG) == -1) {
		perror("Failed to listen");
		exit(1);
	}

	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		if((receiveSockFd = accept(sockfd, (struct sockaddr *)&theirAddr, &sin_size)) == -1) {
			perror("Failed to accept");
			continue;
		}
		printf("Server : got connection from %s\n", inet_ntoa(theirAddr.sin_addr));
	}
}