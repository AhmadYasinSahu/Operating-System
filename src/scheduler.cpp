#include <iostream>
#include <queue>
#include <cstring>
#include <ctime>
#include "../include/scheduler.h"
#include "../include/common.h"

RoundRobinScheduler::RoundRobinScheduler(int quantum) 
    : time_quantum(quantum), current_process(nullptr), total_cpu_time(0) {
    Logger::log("Scheduler", "Round-Robin Scheduler initialized");
}

RoundRobinScheduler::~RoundRobinScheduler() {
    while (!ready_queue.empty()) {
        delete ready_queue.front();
        ready_queue.pop();
    }
}

void RoundRobinScheduler::add_process(PCB* pcb) {
    pcb->state = READY;
    pcb->start_time = time(nullptr);
    ready_queue.push(pcb);
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Process added: %s (ID: %d)", pcb->process_name, pcb->process_id);
    Logger::log("Scheduler", buffer);
}

PCB* RoundRobinScheduler::get_next_process() {
    if (ready_queue.empty()) {
        return nullptr;
    }
    
    PCB* pcb = ready_queue.front();
    ready_queue.pop();
    
    pcb->state = RUNNING;
    current_process = pcb;
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Process scheduled: %s (Time Quantum: %d ms)", pcb->process_name, time_quantum);
    Logger::log("Scheduler", buffer);
    
    return pcb;
}

void RoundRobinScheduler::process_completed(int process_id) {
    if (current_process && current_process->process_id == process_id) {
        current_process->state = COMPLETED;
        current_process->end_time = time(nullptr);
        
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Process completed: %s", current_process->process_name);
        Logger::log("Scheduler", buffer);
        
        current_process = nullptr;
    }
}

void RoundRobinScheduler::print_schedule_status() {
    std::cout << "\n========== SCHEDULER STATUS ==========" << std::endl;
    std::cout << "Queue Size: " << ready_queue.size() << std::endl;
    
    if (current_process) {
        std::cout << "Current Process: " << current_process->process_name 
                  << " (State: RUNNING)" << std::endl;
    }
    
    std::cout << "====================================\n" << std::endl;
}

int RoundRobinScheduler::get_queue_size() {
    return ready_queue.size();
}
