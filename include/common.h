#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <vector>

// Logging utilities
class Logger {
public:
    static void log(const char* module, const char* message);
    static void log_error(const char* module, const char* message);
    static void log_debug(const char* module, const char* message);
    static void log_performance(const char* module, long duration_ms);
};

// Performance metrics
typedef struct {
    char stage_name[32];
    long start_time;
    long end_time;
    long duration;
    int items_processed;
    double throughput;  // items per second
} PerformanceMetric;

// Global configuration
typedef struct {
    int num_threads_per_stage;
    int time_quantum;
    int log_level;  // 0=ERROR, 1=INFO, 2=DEBUG
    char input_file[256];
    char output_dir[256];
    char log_dir[256];
} Config;

#endif // COMMON_H
