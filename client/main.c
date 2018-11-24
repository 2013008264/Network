#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 43134
#define BUF_SIZE 1024

int main() {
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char * hello = "Hello from client";
	char buffer[BUF_SIZE] = {0};

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to make socket");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);

	if(inet_pton(AF_INET, "106.10.54.51", &serv_addr.sin_addr) <= 0) {
		perror("Invalid address");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Failed to connect");
		return -1;
	}

	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = (int)recv(sock, buffer, BUF_SIZE, 0);
	printf("%s\n", buffer);
}