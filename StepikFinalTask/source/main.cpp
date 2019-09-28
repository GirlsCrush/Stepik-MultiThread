#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
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

void httpHandler(int fd) {

}

static const char templ[] = "HTTP/1.0 200 OK\r\n"

		           "Content-length: %d\r\n"

		       	   "Connection: close\r\n"

		       	   "Content-Type: text/html\r\n"

		       	   "\r\n"

		       	   "%s";

static const char not_found[] = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";

int main(int argc, char** argv)
{
    
    if (argc < 2)
    {
        printf("Usage: ./my_daemon -h <ip> -p <port> -d <directory>\n");
        return -1;
    }

    int status;
    uint16_t pid, sid;
    in_addr_t inAddr;
    uint16_t port;
    std::string dir;
    int rez = 0;

    while ((rez = getopt(argc,argv,"h:p:d:")) != -1){
		switch (rez){
		case 'h': 
        printf("IP = %s.\n",optarg);
        inAddr = inet_addr(optarg);
        break;
		case 'p': printf("PORT = %s.\n",optarg); 
        port = htons(atoi(optarg));
        break;
		case 'd': printf("DIRECTORY = %s\n", optarg); 
        dir = optarg;
        break;
		case '?': printf("Error found !\n");break;
        };
	};
    

    
    if (!status) // если произошла ошибка загрузки конфига
    {
        printf("Error: Load config failed\n");
        return -1;
    }
    
    // создаем потомка
    pid = fork();

    if (pid == -1) // если не удалось запустить потомка
    {
        // выведем на экран ошибку и её описание
        printf("Error: Start Daemon failed \n");
        
        return -1;
    }
    else if (!pid) // если это потомок
    {
        // данный код уже выполняется в процессе потомка
        // разрешаем выставлять все биты прав на создаваемые файлы,
        // иначе у нас могут быть проблемы с правами доступа
        umask(0);
        
        // создаём новый сеанс, чтобы не зависеть от родителя
        sid = setsid();
        if (sid < 0) {
            printf("Error: SID is not given.\n");
            exit(EXIT_FAILURE);
        }
        
        // переходим в корень диска, если мы этого не сделаем, то могут быть проблемы.
        // к примеру с размантированием дисков
        if (chdir("/") < 0) {
            printf("Error: Working dir is not changed.\n");
            exit(EXIT_FAILURE);
        }
        
        // закрываем дискрипторы ввода/вывода/ошибок, так как нам они больше не понадобятся
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = port;
	SockAddr.sin_addr.s_addr = inAddr;
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
			} else {
				static char Buffer[1024];
				int RecvSize = recv(Events[i].data.fd, Buffer, 1024, 
				MSG_NOSIGNAL);
				if ((RecvSize == 0) && (errno != EAGAIN)) { 
					shutdown(Events[i].data.fd, SHUT_RDWR);
					close(Events[i].data.fd);
				} else if (RecvSize > 0) {
					FILE *f = fopen("http-request.txt", "w");
					fprintf(f, "%s\n", Buffer);
				}
			}
		}
	}
        
        
        return status;
    }
    else // если это родитель
    {
        // завершим процес, т.к. основную свою задачу (запуск демона) мы выполнили
        return 0;
    }
}