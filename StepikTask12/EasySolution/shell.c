#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main() {
    char buf[BUFFER_SIZE];
    memset((void *)buf, '\0', BUFFER_SIZE);
    fgets(buf, BUFFER_SIZE, stdin);
    FILE* op = popen(buf, "r");
    FILE* f = fopen("/home/box/result.out", "w");
    while (fgets(buf, BUFFER_SIZE, op)) {
        fputs(buf, f);
    }
    fclose(op);
    fclose(f);
    return 0;
}