#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    pid_t child_pid;
	int socket_vector[2];

	if(0 != socketpair(AF_UNIX, SOCK_STREAM, 0, socket_vector)) {
		perror("cannot create socket pair");
		return 2;
	}

	child_pid = fork();
    char r_buf[] = "Hello there!\n";
    char buf[1024];

    if(-1 == child_pid) {
		perror("cannot fork process");
		return 2;
	} else if (0 == child_pid) {
		close(socket_vector[0]);
		read(socket_vector[1], &buf, sizeof(buf));
	} else {
		fprintf(stderr, "PID of child process: %d.\n", (int)(child_pid));
		
		close(socket_vector[1]);
		read(socket_vector[0], &buf, sizeof(buf));
	}
    return 0;
}