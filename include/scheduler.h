#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <ctime>
#include <queue>

#define TIME_QUANTUM 100  // Time quantum for round-robin in milliseconds

// Process state enum
typedef enum {
    READY,
    RUNNING,
    WAITING,
    COMPLETED
} ProcessState;

// Process Control Block
typedef struct {
    int process_id;
    char process_name[64];
    ProcessState state;
    int burst_time;     // Expected execution time
    int cpu_time_used;  // Time already used
    long start_time;
    long end_time;
    int priority;       // 0 = lowest, 10 = highest
} PCB;

// Scheduler class
class RoundRobinScheduler {
private:
    std::queue<PCB*> ready_queue;
    PCB* current_process;
    int time_quantum;
    int total_cpu_time;
    
public:
    RoundRobinScheduler(int quantum = TIME_QUANTUM);
    ~RoundRobinScheduler();
    
    void add_process(PCB* pcb);
    PCB* get_next_process();
    void process_completed(int process_id);
    void print_schedule_status();
    int get_queue_size();
};

#endif // SCHEDULER_H
