#include <sys/types.h>
//

#include <sys/ipc.h>
//

#include <sys/sem.h>
//

#include <unistd.h>
//

#include <stdlib.h>


int main() {
    key_t key = ftok("/tmp/sem.temp", 1);
    int semid = semget(key, 16, 0666 | IPC_CREAT);

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    struct sembuf sem_opt[16];
    for(int i = 0; i < 16; ++i) {
        arg.val = i;
        semctl(semid, i, SETVAL, arg);
    }
    pause();

    return 0;
}