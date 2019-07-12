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

	connect(Socket, (struct sockaddr*)(&address), sizeof(address));

    char buffer[] = "PING";
    char buffer2[] = "____";
    send(Socket, buffer, 4, 0);
    recv(Socket, buffer2, 4, 0);
    shutdown(Socket, SHUT_RDWR);
	//close(Socket);
	printf("%s\n", buffer2);

    return 0;
}