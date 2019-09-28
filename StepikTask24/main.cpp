#include <pthread.h>
//
#include <unistd.h>
//
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cd = PTHREAD_COND_INITIALIZER;
pthread_barrier_t b;

void* funcCond(void *v) {
    pthread_cond_wait(&cd, &m);
    
    return NULL;
}
void* funcBarrier(void *v) {
    pthread_barrier_wait(&b);
    return NULL;
}
int main() {
    FILE *f = fopen("main.pid", "w");
    fprintf(f, "%d", getpid());
    fclose(f);

    pthread_barrier_init(&b, NULL, 2);

    pthread_t pth[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&pth[0], &attr, funcCond, NULL);
    pthread_join(pth[0], NULL);
    pthread_create(&pth[1], &attr, funcBarrier, NULL);
    pthread_join(pth[1], NULL);
    return 0;
}