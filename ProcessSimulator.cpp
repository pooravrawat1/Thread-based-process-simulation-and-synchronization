#include "ProcessSimulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>

ProcessSimulator::ProcessSimulator() {
    // Initialize start time for timestamp tracking
    startTime = std::chrono::steady_clock::now();
}

ProcessSimulator::~ProcessSimulator() {
    // Destructor - cleanup if needed
}

bool ProcessSimulator::loadProcesses(const std::string& filename) {
    std::ifstream file(filename);
    
    // Check if file exists and can be opened
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        std::istringstream iss(line);
        int pid, burstTime;
        
        // Try to read process ID and burst time
        if (!(iss >> pid >> burstTime)) {
            std::cerr << "Error: Invalid data format at line " << lineNumber 
                      << " in file " << filename << std::endl;
            return false;
        }
        
        // Validate process ID (should be positive)
        if (pid <= 0) {
            std::cerr << "Error: Invalid process ID " << pid 
                      << " at line " << lineNumber 
                      << ". Process ID must be positive." << std::endl;
            return false;
        }
        
        // Validate burst time (should be positive)
        if (burstTime <= 0) {
            std::cerr << "Error: Invalid burst time " << burstTime 
                      << " at line " << lineNumber 
                      << ". Burst time must be positive." << std::endl;
            return false;
        }
        
        // Add valid process to the vector
        Process proc;
        proc.pid = pid;
        proc.burstTime = burstTime;
        processes.push_back(proc);
    }
    
    file.close();
    
    // Check if we loaded at least one process
    if (processes.empty()) {
        std::cerr << "Warning: No valid processes found in " << filename << std::endl;
    }
    
    return true;
}

std::string ProcessSimulator::getTimestamp() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    
    double seconds = elapsed.count() / 1000.0;
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << seconds;
    return oss.str();
}

void ProcessSimulator::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[" << getTimestamp() << "s] " << message << std::endl;
}

void ProcessSimulator::processWorker(int pid, int burstTime, ProcessSimulator* sim) {
    // Log process start
    std::ostringstream ss;
    ss << "PROCESS " << std::setw(3) << pid << " | Started (burst time: " << burstTime << "s)";
    sim->log(ss.str());
    
    // Simulate CPU burst time using sleep
    std::this_thread::sleep_for(std::chrono::seconds(burstTime));
    
    // Log process finish
    ss.str("");
    ss << "PROCESS " << std::setw(3) << pid << " | Finished";
    sim->log(ss.str());
}

void ProcessSimulator::executeProcesses() {
    std::vector<std::thread> threads;
    
    // Create a thread for each process
    for (const auto& process : processes) {
        threads.push_back(std::thread(processWorker, process.pid, process.burstTime, this));
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
}
