#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 43134
#define BUF_SIZE 1024
#define BACKLOG 10

int main() {
	int server_fd; // socket file descriptor.
	int new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUF_SIZE] = {0};
	char * hello = "Hello from server";

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
	 if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	 	perror("Failed to make socket");
	 	return -1;
	 }

	 if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
	 	perror("Failed to set socket");
	 	return -1;
	 }

	address.sin_family = AF_INET; // Address family of internet.
	// htons == host to network short
	address.sin_port = htons(PORT);
	// htonl == host to network long
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding socket addr.
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Failed to bind socket");
		return 1;
	}

	// Wait listen.
	if(listen(server_fd, BACKLOG) == -1) {
		perror("Failed to listen");
		exit(1);
	}

	while(1) {
		if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
			perror("Failed to accept");
			return -1;
		}
		valread = (int)recv(new_socket, buffer, BUF_SIZE, 0);
		printf("%s\n", buffer);
		send(new_socket, hello, strlen(hello), 0);
		printf("Hello message sent\n");
	}
}