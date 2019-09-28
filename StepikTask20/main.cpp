#include <sys/ipc.h>
//
#include <sys/types.h>
//
#include <sys/shm.h>
//
#include <string.h>
//
#include <unistd.h>

int main() {
    key_t key = ftok("/tmp/mem.temp", 1);
    int shmid = shmget(key, 2 * 1048576, 0666 | IPC_CREAT);
    char *memory = (char *)shmat(shmid, NULL, 0);
    memset(memory, 42, 1048576);
    pause();
    return 0;
}