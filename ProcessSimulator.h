#ifndef PROCESS_SIMULATOR_H
#define PROCESS_SIMULATOR_H

#include <vector>
#include <mutex>
#include <string>

struct Process {
    int pid;
    int burstTime;
};

class ProcessSimulator {
private:
    std::vector<Process> processes;
    std::mutex coutMutex;

public:
    ProcessSimulator();
    ~ProcessSimulator();
    
    // Load processes from file
    bool loadProcesses(const std::string& filename);
    
    // Thread-safe logging
    void log(const std::string& message);
};

#endif // PROCESS_SIMULATOR_H
