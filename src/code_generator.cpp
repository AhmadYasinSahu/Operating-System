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
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
int message_queue_id = -1;

// Code generation with mutex for file I/O
void* generate_code(void* arg) {
    int thread_id = *(int*)arg;
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Code generator thread %d started", thread_id);
    Logger::log("CodeGenerator", buffer);
    
    // Simulate code generation
    usleep(100000);
    
    // Use mutex to safely write to output file
    pthread_mutex_lock(&file_mutex);
    
    std::ofstream outfile("data/output/code_segment.asm", std::ios::app);
    outfile << "; Code segment " << thread_id << std::endl;
    outfile << "MOV AX, BX" << std::endl;
    outfile << "ADD AX, CX" << std::endl;
    outfile.close();
    
    pthread_mutex_unlock(&file_mutex);
    
    snprintf(buffer, sizeof(buffer), "Code generator thread %d completed", thread_id);
    Logger::log("CodeGenerator", buffer);
    
    pthread_exit(nullptr);
}

int main(int argc, char* argv[]) {
    Logger::log("CodeGenerator", "=== CODE GENERATION PROCESS STARTED ===");
    
    // Attach to shared memory
    shm = attach_shared_memory();
    if (!shm) {
        Logger::log_error("CodeGenerator", "Failed to attach shared memory");
        return 1;
    }
    
    // Get message queue
    message_queue_id = get_message_queue(MQ_KEY_BASE + 1);
    
    // Create output file
    std::ofstream outfile("data/output/code_segment.asm");
    outfile << "; Generated Assembly Code" << std::endl;
    outfile << "; Generated at: " << time(nullptr) << std::endl;
    outfile.close();
    
    // Create threads for code generation
    pthread_t threads[2];
    int thread_ids[2];
    
    Logger::log("CodeGenerator", "Creating code generator threads...");
    
    for (int i = 0; i < 2; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, generate_code, &thread_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    // Update shared memory status
    strcpy(shm->status, "CODE_GENERATION_COMPLETE");
    
    // Send completion message
    Message msg;
    msg.msg_type = 6;  // Supervisor
    strcpy(msg.stage, "CodeGenerator");
    strcpy(msg.data, "Code generation complete. Output file generated.");
    msg.timestamp = time(nullptr);
    
    if (message_queue_id >= 0) {
        send_message(message_queue_id, &msg);
    }
    
    std::cout << "\n========== CODE GENERATION RESULTS ==========" << std::endl;
    std::cout << "Assembly Code Generated: YES" << std::endl;
    std::cout << "Output File: data/output/code_segment.asm" << std::endl;
    std::cout << "Code Size: ~1KB" << std::endl;
    std::cout << "============================================\n" << std::endl;
    
    Logger::log("CodeGenerator", "=== CODE GENERATION PROCESS FINISHED ===");
    
    // Detach shared memory
    detach_shared_memory(shm);
    
    return 0;
}
