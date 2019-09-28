#include <pthread.h>
//
#include <unistd.h>
//
#include <stdio.h>


void* func(void *v) {
    FILE *f = fopen("main.pid", "w");
    fprintf(f, "%d", getpid());
    fclose(f);
    pause();
    return NULL;
}

int main() {
    pthread_t pth;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&pth, &attr, func, NULL);
    pthread_join(pth, NULL);
    return 0;
}