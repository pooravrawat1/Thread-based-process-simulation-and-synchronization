#ifndef PROCESS_SIMULATOR_H
#define PROCESS_SIMULATOR_H

#include <vector>
#include <mutex>
#include <string>
#include <chrono>

struct Process {
    int pid;
    int burstTime;
};

class ProcessSimulator {
private:
    std::vector<Process> processes;
    std::mutex coutMutex;
    std::chrono::steady_clock::time_point startTime;

public:
    ProcessSimulator();
    ~ProcessSimulator();
    
    // Load processes from file
    bool loadProcesses(const std::string& filename);
    
    // Execute all process threads
    void executeProcesses();
    
    // Thread-safe logging with timestamp
    void log(const std::string& message);
    
    // Get elapsed time since start
    std::string getTimestamp();

private:
    // Worker function for each process thread
    static void processWorker(int pid, int burstTime, ProcessSimulator* sim);
};

#endif // PROCESS_SIMULATOR_H
