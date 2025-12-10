#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <pthread.h>
#include "../include/shared_memory.h"
#include "../include/message_queue.h"
#include "../include/common.h"

SharedMemory* shm = nullptr;
int semantic_errors = 0;
int message_queue_id = -1;

// Semantic analysis thread
void* semantic_check(void* arg) {
    int thread_id = *(int*)arg;
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Semantic checker thread %d started", thread_id);
    Logger::log("SemanticAnalyzer", buffer);
    
    // Simulate semantic checking
    for (int i = 0; i < 5; i++) {
        // Check type consistency, scope, etc.
        usleep(75000);
    }
    
    snprintf(buffer, sizeof(buffer), "Semantic checker thread %d completed", thread_id);
    Logger::log("SemanticAnalyzer", buffer);
    
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    Logger::log("SemanticAnalyzer", "=== SEMANTIC ANALYSIS PROCESS STARTED ===");
    
    // Attach to shared memory
    shm = attach_shared_memory();
    if (!shm) {
        Logger::log_error("SemanticAnalyzer", "Failed to attach shared memory");
        return 1;
    }
    
    // Get message queue
    message_queue_id = get_message_queue(MQ_KEY_BASE + 1);
    
    // Create thread for semantic analysis
    pthread_t thread;
    int thread_id = 0;
    
    Logger::log("SemanticAnalyzer", "Starting semantic analysis...");
    pthread_create(&thread, nullptr, semantic_check, &thread_id);
    pthread_join(thread, nullptr);
    
    // Update shared memory status
    strcpy(shm->status, "SEMANTIC_ANALYSIS_COMPLETE");
    
    // Send message to next stage (Code Generator)
    Message msg;
    msg.msg_type = 5;  // Code generator
    strcpy(msg.stage, "SemanticAnalyzer");
    snprintf(msg.data, sizeof(msg.data), "Semantic check complete. Errors: %d", semantic_errors);
    msg.timestamp = time(nullptr);
    
    if (message_queue_id >= 0) {
        send_message(message_queue_id, &msg);
    }
    
    std::cout << "\n========== SEMANTIC ANALYSIS RESULTS ==========" << std::endl;
    std::cout << "Type Checking: PASSED" << std::endl;
    std::cout << "Scope Analysis: PASSED" << std::endl;
    std::cout << "Semantic Errors Found: " << semantic_errors << std::endl;
    std::cout << "=============================================\n" << std::endl;
    
    Logger::log("SemanticAnalyzer", "=== SEMANTIC ANALYSIS PROCESS FINISHED ===");
    
    // Detach shared memory
    detach_shared_memory(shm);
    
    return 0;
}
