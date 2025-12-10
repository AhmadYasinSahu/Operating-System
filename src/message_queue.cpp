#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <ctime>
#include "../include/message_queue.h"
#include "../include/common.h"

// Create message queue
int create_message_queue(int key) {
    int msqid = msgget(key, IPC_CREAT | 0666);
    
    if (msqid < 0) {
        Logger::log_error("MessageQueue", "Failed to create message queue");
        return -1;
    }
    
    Logger::log("MessageQueue", "Message queue created successfully");
    return msqid;
}

// Get existing message queue
int get_message_queue(int key) {
    int msqid = msgget(key, 0666);
    
    if (msqid < 0) {
        Logger::log_error("MessageQueue", "Failed to get message queue");
        return -1;
    }
    
    return msqid;
}

// Send message to queue
int send_message(int msqid, Message* msg) {
    if (msgsnd(msqid, msg, sizeof(Message) - sizeof(long), 0) < 0) {
        Logger::log_error("MessageQueue", "Failed to send message");
        return -1;
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Message sent from %s to queue", msg->stage);
    Logger::log("MessageQueue", buffer);
    
    return 0;
}

// Receive message from queue
int receive_message(int msqid, Message* msg, long msg_type) {
    if (msgrcv(msqid, msg, sizeof(Message) - sizeof(long), msg_type, 0) < 0) {
        Logger::log_error("MessageQueue", "Failed to receive message");
        return -1;
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Message received: %s", msg->data);
    Logger::log("MessageQueue", buffer);
    
    return 0;
}

// Destroy message queue
void destroy_message_queue(int msqid) {
    if (msgctl(msqid, IPC_RMID, nullptr) < 0) {
        Logger::log_error("MessageQueue", "Failed to destroy message queue");
    }
}
