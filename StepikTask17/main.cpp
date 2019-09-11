#include <stdio.h>
//
#include <stdlib.h>
//

#include <sys/stat.h>
//
#include <string.h>
//

#include <mqueue.h>
//

#include <mqueue.h>
//

#define QUEUE_MAXMSG  16 /* Maximum number of messages. */
//
#define QUEUE_MSGSIZE 1024 /* Length of message. */
//
#define QUEUE_ATTR_INITIALIZER ((struct mq_attr){0, QUEUE_MAXMSG, QUEUE_MSGSIZE, 0})
//



int main() {
    struct mq_attr attr = QUEUE_ATTR_INITIALIZER;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    mqd_t mqid = mq_open("/test.mq", O_RDONLY | O_CREAT | O_EXCL, 0666, &attr);
    mq_receive(mqid, buffer, sizeof(buffer), 0);
    FILE* f = fopen("/home/box/message.txt", "w+");

    printf("File = %d\n", f);
    
    fprintf(f, buffer);

    return 0;
}