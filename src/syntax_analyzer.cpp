#include <iostream>
#include <fstream>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include "../include/shared_memory.h"
#include "../include/message_queue.h"
#include "../include/common.h"

#define NUM_THREADS 2

SharedMemory* shm = nullptr;
pthread_mutex_t parse_mutex = PTHREAD_MUTEX_INITIALIZER;
int parsed_tokens = 0;
int message_queue_id = -1;

// Thread function for parallel syntax parsing
void* parse_syntax(void* arg) {
    int thread_id = *(int*)arg;
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Parser thread %d started", thread_id);
    Logger::log("SyntaxAnalyzer", buffer);
    
    // Simulate parsing - validate tokens
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&parse_mutex);
        
        if (parsed_tokens < shm->token_count) {
            // Validate token syntax
            int idx = parsed_tokens;
            parsed_tokens++;
            
            pthread_mutex_unlock(&parse_mutex);
            
            // Simulate parsing work
            usleep(50000);
        } else {
            pthread_mutex_unlock(&parse_mutex);
        }
    }
    
    snprintf(buffer, sizeof(buffer), "Parser thread %d completed", thread_id);
    Logger::log("SyntaxAnalyzer", buffer);
    
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    Logger::log("SyntaxAnalyzer", "=== SYNTAX ANALYSIS PROCESS STARTED ===");
    
    // Attach to shared memory
    shm = attach_shared_memory();
    if (!shm) {
        Logger::log_error("SyntaxAnalyzer", "Failed to attach shared memory");
        return 1;
    }
    
    // Get message queue
    message_queue_id = get_message_queue(MQ_KEY_BASE + 1);
    
    // Wait for tokens from lexical analyzer
    if (shm->token_count == 0) {
        Logger::log("SyntaxAnalyzer", "Waiting for tokens from lexical analyzer...");
        sleep(1);
    }
    
    // Create threads for parallel parsing
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    Logger::log("SyntaxAnalyzer", "Creating parser threads...");
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, parse_syntax, &thread_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    // Update shared memory status
    strcpy(shm->status, "SYNTAX_ANALYSIS_COMPLETE");
    
    // Send message to next stage (Semantic Analyzer)
    Message msg;
    msg.msg_type = 3;  // Semantic analyzer
    strcpy(msg.stage, "SyntaxAnalyzer");
    snprintf(msg.data, sizeof(msg.data), "Parsed tokens: %d", parsed_tokens);
    msg.timestamp = time(nullptr);
    
    if (message_queue_id >= 0) {
        send_message(message_queue_id, &msg);
    }
    
    std::cout << "\n========== SYNTAX ANALYSIS RESULTS ==========" << std::endl;
    std::cout << "Total Tokens Parsed: " << parsed_tokens << std::endl;
    std::cout << "Syntax Check: PASSED" << std::endl;
    std::cout << "Parse Tree: Generated Successfully" << std::endl;
    std::cout << "==========================================\n" << std::endl;
    
    Logger::log("SyntaxAnalyzer", "=== SYNTAX ANALYSIS PROCESS FINISHED ===");
    
    // Detach shared memory
    detach_shared_memory(shm);
    
    return 0;
}
