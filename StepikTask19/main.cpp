#include <fcntl.h>           
//

#include <sys/stat.h>        

//
#include <semaphore.h>
//
#include <unistd.h>


int main() {
    sem_unlink("/test.sem");
    sem_t *semid = sem_open("/test.sem", 0666 | O_CREAT, 0, 66);
    pause();
    return 0;
}