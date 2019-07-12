#include <stdio.h>

#include <sys/types.h>        
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char** argv) {
	int Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(12345);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(Socket, (struct sockaddr*)(&address), sizeof(address));

	listen(Socket, SOMAXCONN);
	while (1) {
		int client_fd = accept(Socket, 0, 0);

		char buffer[5] = { 0 , 0 , 0 , 0 , 0 };
		recv(client_fd, buffer, 4, MSG_NOSIGNAL);
	    send(client_fd, buffer, 4, 0);
		shutdown(client_fd, SHUT_RDWR);
		//close(client_fd);
		printf("%s\n", buffer);
	}
	return 0;
}