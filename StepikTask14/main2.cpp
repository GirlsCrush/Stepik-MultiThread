#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    int fd_fifo_in;
    int fd_fifo_out;
    char buffer[] = "HELLO\n";
    
    if((fd_fifo_in = open("in.fifo", O_WRONLY | O_NONBLOCK)) == -1) {
        fprintf(stderr, "Невозможно открыть infifo\n");
        exit(0);
    }

    printf("END1\n");
    
    
    write(fd_fifo_in, buffer, sizeof(buffer));
}