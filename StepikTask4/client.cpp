#include <Winsock2.h>
#include <stdio.h>

int main(int argc, char** argv) { 
    WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

    int Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(12345);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	connect(Socket, (struct sockaddr*)(&address), sizeof(address));

    char buffer[] = "PING";
    char buffer2[4];
    send(Socket, buffer, 4, 0);
    recv(Socket, buffer2, 4, 0);
    shutdown(Socket, SD_BOTH);
	closesocket(Socket);
	printf("%s\n", buffer2);

    return 0;
}