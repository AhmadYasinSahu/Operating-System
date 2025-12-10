#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <cstdlib>
#include "../include/shared_memory.h"
#include "../include/common.h"

// Create and initialize shared memory
SharedMemory* create_shared_memory() {
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    
    if (shm_id < 0) {
        Logger::log_error("SharedMemory", "Failed to create shared memory");
        return nullptr;
    }
    
    SharedMemory* shm = (SharedMemory*)shmat(shm_id, nullptr, 0);
    
    if (shm == (SharedMemory*)-1) {
        Logger::log_error("SharedMemory", "Failed to attach shared memory");
        return nullptr;
    }
    
    init_shared_memory(shm);
    Logger::log("SharedMemory", "Shared memory created successfully");
    
    return shm;
}

// Attach to existing shared memory
SharedMemory* attach_shared_memory() {
    int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666);
    
    if (shm_id < 0) {
        Logger::log_error("SharedMemory", "Failed to get shared memory ID");
        return nullptr;
    }
    
    SharedMemory* shm = (SharedMemory*)shmat(shm_id, nullptr, 0);
    
    if (shm == (SharedMemory*)-1) {
        Logger::log_error("SharedMemory", "Failed to attach to shared memory");
        return nullptr;
    }
    
    return shm;
}

// Detach from shared memory
void detach_shared_memory(SharedMemory* shm) {
    if (shmdt(shm) < 0) {
        Logger::log_error("SharedMemory", "Failed to detach shared memory");
    }
}

// Destroy shared memory
void destroy_shared_memory(int shm_id) {
    if (shmctl(shm_id, IPC_RMID, nullptr) < 0) {
        Logger::log_error("SharedMemory", "Failed to destroy shared memory");
    }
}

// Initialize shared memory
void init_shared_memory(SharedMemory* shm) {
    shm->token_count = 0;
    shm->memory_used = 0;
    strcpy(shm->status, "INITIALIZED");
    memset(shm->tokens, 0, sizeof(shm->tokens));
    Logger::log("SharedMemory", "Shared memory initialized");
}
