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

#define NUM_THREADS 3

SharedMemory* shm = nullptr;
pthread_mutex_t token_mutex = PTHREAD_MUTEX_INITIALIZER;
int message_queue_id = -1;

// Thread function for parallel tokenization
void* tokenize_chunk(void* arg) {
    int thread_id = *(int*)arg;
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Tokenizer thread %d started", thread_id);
    Logger::log("LexicalAnalyzer", buffer);
    
    // Simulate tokenization
    for (int i = 0; i < 5; i++) {
        Token token;
        snprintf(token.type, sizeof(token.type), "KEYWORD");
        snprintf(token.value, sizeof(token.value), "int");
        token.line_no = thread_id * 5 + i + 1;
        token.col_no = i * 10 + 1;
        
        pthread_mutex_lock(&token_mutex);
        
        if (shm->token_count < MAX_TOKENS) {
            shm->tokens[shm->token_count] = token;
            shm->token_count++;
            shm->memory_used += sizeof(Token);
        }
        
        pthread_mutex_unlock(&token_mutex);
        
        usleep(100000);  // Simulate processing time
    }
    
    snprintf(buffer, sizeof(buffer), "Tokenizer thread %d completed", thread_id);
    Logger::log("LexicalAnalyzer", buffer);
    
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    Logger::log("LexicalAnalyzer", "=== LEXICAL ANALYSIS PROCESS STARTED ===");
    
    // Attach to shared memory
    shm = attach_shared_memory();
    if (!shm) {
        Logger::log_error("LexicalAnalyzer", "Failed to attach shared memory");
        return 1;
    }
    
    // Get or create message queue
    message_queue_id = get_message_queue(MQ_KEY_BASE + 1);
    if (message_queue_id < 0) {
        message_queue_id = create_message_queue(MQ_KEY_BASE + 1);
    }
    
    // Create threads for parallel tokenization
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    Logger::log("LexicalAnalyzer", "Creating tokenizer threads...");
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, tokenize_chunk, &thread_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    // Update shared memory status
    strcpy(shm->status, "LEXICAL_ANALYSIS_COMPLETE");
    
    // Send message to next stage (Syntax Analyzer)
    Message msg;
    msg.msg_type = 2;  // Syntax analyzer
    strcpy(msg.stage, "LexicalAnalyzer");
    snprintf(msg.data, sizeof(msg.data), "Tokens: %d, Memory Used: %d bytes", 
             shm->token_count, shm->memory_used);
    msg.timestamp = time(nullptr);
    
    if (message_queue_id >= 0) {
        send_message(message_queue_id, &msg);
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Lexical analysis complete. Total tokens: %d", shm->token_count);
    Logger::log("LexicalAnalyzer", buffer);
    
    // Print token samples
    std::cout << "\n========== LEXICAL ANALYSIS RESULTS ==========" << std::endl;
    std::cout << "Total Tokens Generated: " << shm->token_count << std::endl;
    std::cout << "Memory Used: " << shm->memory_used << " bytes" << std::endl;
    std::cout << "\nFirst 5 Tokens:" << std::endl;
    for (int i = 0; i < 5 && i < shm->token_count; i++) {
        std::cout << "  Token " << (i+1) << ": Type=" << shm->tokens[i].type 
                  << ", Value=" << shm->tokens[i].value 
                  << ", Line=" << shm->tokens[i].line_no << std::endl;
    }
    std::cout << "=============================================\n" << std::endl;
    
    Logger::log("LexicalAnalyzer", "=== LEXICAL ANALYSIS PROCESS FINISHED ===");
    
    // Detach shared memory
    detach_shared_memory(shm);
    
    return 0;
}
