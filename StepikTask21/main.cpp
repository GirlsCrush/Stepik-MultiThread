#include <sys/mman.h>
//
#include <sys/stat.h>        /* For mode constants */
//
#include <fcntl.h>
//
#include <unistd.h>
//
#include <sys/types.h>
//
#include <string.h>
//
#define SEG_SIZE 1050000


int main() {
    int shmid = shm_open("/test.shm", O_CREAT | O_EXCL | O_RDWR, 0);
    ftruncate(shmid, SEG_SIZE);

    char *mem = (char *)mmap(NULL, SEG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);

    memset((void *)mem, 13, SEG_SIZE);
    pause();
    return 0;
}