    #include <sys/types.h>
    //
    #include <sys/stat.h>
    //
    #include <unistd.h>
    //
    #include <stdio.h>
    //
    #include <stdlib.h>
    //
    #include <fcntl.h>
    //
    #include <string>


    int main() {
        int fd_fifo_in;
        int fd_fifo_out;
        char buffer[1024];
        if((mkfifo("/home/box/in.fifo", 0666)) == -1) {
            fprintf(stderr, "Невозможно создать infifo\n");
            exit(0);
        } 
            printf("mkfifo in\n");

        if((fd_fifo_in = open("/home/box/in.fifo", O_RDONLY | O_NONBLOCK)) == -1) {
            fprintf(stderr, "Невозможно открыть infifo\n");
            exit(0);
        }
            printf("opened in\n");


        if((mkfifo("/home/box/out.fifo", 0666)) == -1) {
            fprintf(stderr, "Невозможно создать outfifo\n");
            exit(0);
        } 
        if((fd_fifo_out = open("/home/box/out.fifo", O_WRONLY )) == - 1) {
            fprintf(stderr, "Невозможно открыть outfifo\n");
            exit(0);
        }
            printf("opened out\n");


        size_t len = 0;
        std::string buf;
        while (!len) {
            //printf("trying to read\n");
            if ((len = read(fd_fifo_in, &buffer, sizeof(buffer))) == -1) {
                fprintf(stderr, "Невозможно прочесть из FIFO\n"); 
                close(fd_fifo_in);
                close(fd_fifo_out);
                exit(0);
            }
        }
        while (len) {
            buf += buffer;
            if ((len = read(fd_fifo_in, &buffer, sizeof(buffer))) == -1) {
                fprintf(stderr, "Невозможно прочесть из FIFO\n"); 
                close(fd_fifo_in);
                close(fd_fifo_out);
                exit(0);
            }
        }
        printf(buf.c_str());


        
        write(fd_fifo_out, buf.c_str(), buf.size());
        close(fd_fifo_in);
        close(fd_fifo_out);
        
    }