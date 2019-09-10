#include<sys/types.h>
//
#include<sys/ipc.h>
//
#include<sys/msg.h>
//
#include <stdio.h>
//
#define MSG_PERM 0666

struct message {
    long mtype;
    char mtext[80];
};

int main() {
    int msgid;
    key_t key = ftok("/tmp/msg.temp", 1);
    printf("KEY = %d\n", key);
    msgid = msgget(key, IPC_CREAT | MSG_PERM);
    printf("MSGID = %d\n", msgid);
    message msg;

    msgrcv(msgid, (struct message*)&msg, 80, 0, 0);

    printf("MSGRCV\n");

    FILE* f = fopen("/home/box/message.txt", "w+");

    printf("File = %d\n", f);
    
    fprintf(f, msg.mtext);

    return 0;
}