#include <stdio.h>

#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>     
#include <set>        

int set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}
int main(int argc, char** argv) {
	
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	std::set<int> SlaveSockets;
	
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(MasterSocket, (struct sockaddr*)(&SockAddr), sizeof(SockAddr));

	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

	while (true) {
		fd_set Set;
		FD_ZERO(&Set):
		FD_SET(MasterSocket, &Set);
		for (auto iter = SlaveSockets.begin();
			iter != SlaveSockets.end();
			iter++)
			FD_SET(*iter, &Set);
		int Max = *SlaveSockets.rbegin();
		int client_fd = accept(MasterSocket, 0, 0);

		select(Max + 1, &Set, NULL, NULL, NULL);

		char buffer[5] = { 0 , 0 , 0 , 0 , 0 };
		recv(client_fd, buffer, 4, MSG_NOSIGNAL);
	    send(client_fd, buffer, 4, 0);
		shutdown(client_fd, SHUT_RDWR);
		//close(client_fd);
		printf("%s\n", buffer);
	}
	return 0;
}