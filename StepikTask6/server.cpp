#include <stdio.h>
#include <set>
#include <string>
#include <cerrno>

#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define POLL_SIZE 2048

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
	bind(MasterSocket, (struct sockaddr*)(&SockAddr), 
		sizeof(SockAddr));

	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

	struct pollfd Set[POLL_SIZE];
	Set[0].fd = MasterSocket;
	Set[0].events = POLLIN;


	while (true) {
		unsigned int index = 1;
		for (auto iter = SlaveSockets.begin(); 
			iter != SlaveSockets.end(); ++iter, ++index){
			Set[index].fd = *iter;
			Set[index].events = POLLIN;
		}
		size_t setSize = 1 + SlaveSockets.size();

		poll(Set, setSize, -1);
		
		for (size_t i = 0; i < setSize; ++i) {
			if (Set[i].revents & POLLIN) {
				if (i) {
					static char Buffer[1024];
					int RecvSize = recv(Set[i].fd, Buffer, 1024, 
					MSG_NOSIGNAL);
					if ((RecvSize == 0) && (errno != EAGAIN)) { 
						shutdown(Set[i].fd, SHUT_RDWR);
						close(Set[i].fd);
						SlaveSockets.erase(Set[i].fd);
						continue;
					} else if (RecvSize != 0) {
						for (auto iter2 = SlaveSockets.begin();
							iter2 != SlaveSockets.end();
							iter2++) {
							if (Set[i].fd != *iter2)
								send(*iter2, Buffer, RecvSize, MSG_NOSIGNAL);
						}
						printf("%s", std::string(Buffer, RecvSize).
							c_str());
					}
				} else {
					int SlaveSocket = accept(MasterSocket, 0, 0);
					set_nonblock(SlaveSocket);
					SlaveSockets.insert(SlaveSocket);
				}
			}
		}
	}
	return 0;
}