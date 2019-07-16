#include <stdio.h>
#include <set>
#include <string>
#include <cerrno>

#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>     

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
	fd_set Set;

	while (true) {
		FD_ZERO(&Set);
		FD_SET(MasterSocket, &Set);
		for (auto iter = SlaveSockets.begin();
			iter != SlaveSockets.end();
			iter++) {
			FD_SET(*iter, &Set);
		}
		int Max = MasterSocket;
		if(!SlaveSockets.empty() && Max < *SlaveSockets.rbegin()) {
			Max = *SlaveSockets.rbegin();
		}
		select(Max + 1, &Set, NULL, NULL, NULL);
		
		if (FD_ISSET(MasterSocket, &Set)) {
			int SlaveSocket = accept(MasterSocket, 0, 0);
			for (auto iter = SlaveSockets.begin();
					iter != SlaveSockets.end();
					iter++) {
					send(*iter, "A new person is here!\n", 22, MSG_NOSIGNAL);
				}
			send(MasterSocket, "A new person is here!\n", 22, MSG_NOSIGNAL);
			
			set_nonblock(SlaveSocket);
			SlaveSockets.insert(SlaveSocket);
		}
		auto iter = SlaveSockets.begin();
		while (iter != SlaveSockets.end()) {
			if (FD_ISSET(*iter, &Set)) {
				static char Buffer[1024];
				int RecvSize = recv(*iter, Buffer, 1024, 
				MSG_NOSIGNAL);
				if ((RecvSize == 0) && (errno != EAGAIN)) { 
					shutdown(*iter, SHUT_RDWR);
					close(*iter);
					iter = SlaveSockets.erase(iter);
					continue;
				} else if (RecvSize != 0) {
					for (auto iter2 = SlaveSockets.begin();
						iter2 != SlaveSockets.end();
						iter2++) {
						if (iter != iter2)
							send(*iter2, Buffer, RecvSize, MSG_NOSIGNAL);
						printf("%s", std::string(Buffer, RecvSize).
							c_str());
					}
				}
			}
			iter++;
		}

	}
	return 0;
}