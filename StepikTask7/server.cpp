#include <stdio.h>
#include <set>
#include <string>
#include <cerrno>

#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENTS 32

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

	int EPoll = epoll_create1(0);

	struct epoll_event Event;
	Event.data.fd = MasterSocket;
	Event.events = EPOLLIN;
	epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event);
	while (true) {
		struct epoll_event Events[MAX_EVENTS];
		int N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);
		
		for (size_t i = 0; i < N; ++i) {
			if (Events[i].data.fd == MasterSocket) {
				int SlaveSocket = accept(MasterSocket, 0, 0);
				set_nonblock(SlaveSocket);
				
				struct epoll_event Event;
				Event.data.fd = SlaveSocket;
				Event.events = EPOLLIN;
				epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket,
					&Event);
				SlaveSockets.insert(SlaveSocket);
			} else {
				static char Buffer[1024];
				int RecvSize = recv(Events[i].data.fd, Buffer, 1024, 
				MSG_NOSIGNAL);
				if ((RecvSize == 0) && (errno != EAGAIN)) { 
					shutdown(Events[i].data.fd, SHUT_RDWR);
					close(Events[i].data.fd);
					SlaveSockets.erase(Events[i].data.fd);
				} else if (RecvSize > 0) {
					for (auto iter = SlaveSockets.begin(); iter != SlaveSockets.end(); ++iter) {
						if (Events[i].data.fd != *iter)
							send(*iter, Buffer, RecvSize, MSG_NOSIGNAL);
					}
					printf("%s", std::string(Buffer, RecvSize).
						c_str());
				}
			}
		}
	}
	return 0;
}