#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <cstring>
#include <ctime>

#define MQ_KEY_BASE 2000
#define MAX_MESSAGE_SIZE 1024

// Message structure for inter-process communication
typedef struct {
    long msg_type;  // Message type: 1=Lexical, 2=Syntax, 3=Semantic, 4=Optimization, 5=CodeGen
    char stage[32];  // Source stage name
    char data[MAX_MESSAGE_SIZE];  // Message data
    int priority;    // Priority level
    long timestamp;  // Creation timestamp
} Message;

// Message queue operations
int create_message_queue(int key);
int get_message_queue(int key);
int send_message(int msqid, Message* msg);
int receive_message(int msqid, Message* msg, long msg_type);
void destroy_message_queue(int msqid);

#endif // MESSAGE_QUEUE_H
