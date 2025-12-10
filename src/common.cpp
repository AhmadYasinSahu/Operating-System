#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "../include/common.h"

// Logger implementation
void Logger::log(const char* module, const char* message) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::cout << "[" << timestamp << "] [" << module << "] " << message << std::endl;
    
    // Also write to log file
    std::ofstream logfile("logs/compiler_simulator.log", std::ios::app);
    logfile << "[" << timestamp << "] [" << module << "] " << message << std::endl;
    logfile.close();
}

void Logger::log_error(const char* module, const char* message) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::cerr << "[ERROR] [" << timestamp << "] [" << module << "] " << message << std::endl;
    
    std::ofstream logfile("logs/compiler_simulator.log", std::ios::app);
    logfile << "[ERROR] [" << timestamp << "] [" << module << "] " << message << std::endl;
    logfile.close();
}

void Logger::log_debug(const char* module, const char* message) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::cout << "[DEBUG] [" << timestamp << "] [" << module << "] " << message << std::endl;
    
    std::ofstream logfile("logs/compiler_simulator.log", std::ios::app);
    logfile << "[DEBUG] [" << timestamp << "] [" << module << "] " << message << std::endl;
    logfile.close();
}

void Logger::log_performance(const char* module, long duration_ms) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[PERF] [%s] [%s] Duration: %ld ms", timestamp, module, duration_ms);
    
    std::cout << buffer << std::endl;
    
    std::ofstream logfile("logs/compiler_simulator.log", std::ios::app);
    logfile << buffer << std::endl;
    logfile.close();
}
