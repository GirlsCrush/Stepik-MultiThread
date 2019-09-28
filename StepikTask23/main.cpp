#include <pthread.h>
//
#include <unistd.h>
//
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_spinlock_t sl;
pthread_rwlock_t rwl = PTHREAD_RWLOCK_INITIALIZER;

void* func(void *v) {
    pthread_mutex_lock(&m);    
    return NULL;
}
void* funcSpinLock(void *v) {
    pthread_spin_lock(&sl);
    return NULL;
}
void* funcRD(void *v) {
    pthread_rwlock_rdlock(&rwl);
    return NULL;
}
void* funcWR(void *v) {
    pthread_rwlock_wrlock(&rwl);
    return NULL;
}

int main() {
    FILE *f = fopen("main.pid", "w");
    fprintf(f, "%d", getpid());
    fclose(f);

    pthread_spin_init(&sl, PTHREAD_PROCESS_SHARED);
    pthread_spin_lock(&sl);

    pthread_mutex_lock(&m);

    pthread_rwlock_rdlock(&rwl);
    pthread_t pth[4];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&pth[0], &attr, func, NULL);
    pthread_join(pth[0], NULL);
    pthread_create(&pth[1], &attr, funcSpinLock, NULL);
    pthread_join(pth[1], NULL);
    pthread_create(&pth[2], &attr, funcRD, NULL);
    pthread_join(pth[2], NULL);
    pthread_create(&pth[3], &attr, funcWR, NULL);
    pthread_join(pth[3], NULL);
    return 0;
}