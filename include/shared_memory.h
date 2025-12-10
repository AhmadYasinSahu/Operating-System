#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <cstring>
#include <cstdlib>

#define MAX_TOKENS 10000
#define MAX_TOKEN_SIZE 256
#define SHM_KEY 1234
#define SHM_SIZE sizeof(SharedMemory)

// Token structure
typedef struct {
    char type[32];      // Token type (KEYWORD, IDENTIFIER, NUMBER, OPERATOR, etc.)
    char value[MAX_TOKEN_SIZE];  // Token value
    int line_no;
    int col_no;
} Token;

// Shared memory structure for tokens
typedef struct {
    Token tokens[MAX_TOKENS];
    int token_count;
    char status[64];    // Status of lexical analysis
    int memory_used;
} SharedMemory;

// Function declarations
SharedMemory* create_shared_memory();
SharedMemory* attach_shared_memory();
void detach_shared_memory(SharedMemory* shm);
void destroy_shared_memory(int shm_id);
void init_shared_memory(SharedMemory* shm);

#endif // SHARED_MEMORY_H
