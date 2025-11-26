/**
 * @file ProcessSimulator.cpp
 * @brief Implementation of the ProcessSimulator class
 * 
 * This file implements the process simulation functionality, including:
 * - Loading process data from file with validation
 * - Creating and managing process threads
 * - Thread-safe logging with timestamps
 * - CPU burst time simulation using sleep
 * 
 * Thread Safety:
 * - All console output is protected by coutMutex (CRITICAL SECTION)
 * - Uses std::lock_guard for RAII-style automatic mutex unlocking
 * 
 * @author Thread Simulation System
 * @date 2024
 */

#include "ProcessSimulator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>

/**
 * @brief Constructor - initializes start time for timestamp tracking
 */
ProcessSimulator::ProcessSimulator() {
    // Initialize start time for timestamp tracking
    startTime = std::chrono::steady_clock::now();
}

/**
 * @brief Destructor - cleanup resources if needed
 */
ProcessSimulator::~ProcessSimulator() {
    // Destructor - cleanup if needed
}

/**
 * @brief Load processes from a text file with validation
 * 
 * File format: Each line contains "pid burstTime" (space-separated integers)
 * Example:
 *   1 3
 *   2 5
 *   3 2
 * 
 * Validation:
 * - Process ID must be positive
 * - Burst time must be positive
 * - File must exist and be readable
 * 
 * @param filename Path to the input file
 * @return true if file loaded successfully, false on error
 */
bool ProcessSimulator::loadProcesses(const std::string& filename) {
    std::ifstream file(filename);
    
    // Check if file exists and can be opened
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int lineNumber = 0;
    
    // Read file line by line
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

/**
 * @brief Get elapsed time since simulation start
 * 
 * Calculates the time elapsed since the ProcessSimulator was created.
 * Used for timestamp logging to show the sequence of events.
 * 
 * @return Formatted timestamp string in seconds (e.g., "1.234")
 */
std::string ProcessSimulator::getTimestamp() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    
    double seconds = elapsed.count() / 1000.0;
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << seconds;
    return oss.str();
}

/**
 * @brief Thread-safe logging with timestamp
 * 
 * CRITICAL SECTION: This method acquires coutMutex before writing to console.
 * This prevents interleaved output when multiple threads log simultaneously.
 * 
 * Uses std::lock_guard for RAII-style automatic mutex unlocking when the
 * function exits (even if an exception is thrown).
 * 
 * @param message The message to log to console
 */
void ProcessSimulator::log(const std::string& message) {
    // CRITICAL SECTION BEGIN - acquire lock for thread-safe console output
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[" << getTimestamp() << "s] " << message << std::endl;
    // CRITICAL SECTION END - lock automatically released when lock_guard goes out of scope
}

/**
 * @brief Worker function executed by each process thread
 * 
 * This static function is the entry point for each process thread.
 * It performs the following steps:
 * 1. Log process start with burst time
 * 2. Simulate CPU burst time using std::this_thread::sleep_for
 * 3. Log process completion
 * 
 * Thread Safety: Uses sim->log() which is thread-safe
 * 
 * @param pid Process ID
 * @param burstTime CPU burst time in seconds
 * @param sim Pointer to ProcessSimulator instance for thread-safe logging
 */
void ProcessSimulator::processWorker(int pid, int burstTime, ProcessSimulator* sim) {
    // Log process start
    std::ostringstream ss;
    ss << "PROCESS " << std::setw(3) << pid << " | Started (burst time: " << burstTime << "s)";
    sim->log(ss.str());
    
    // Simulate CPU burst time using sleep
    // This represents the process executing on the CPU
    std::this_thread::sleep_for(std::chrono::seconds(burstTime));
    
    // Log process finish
    ss.str("");
    ss << "PROCESS " << std::setw(3) << pid << " | Finished";
    sim->log(ss.str());
}

/**
 * @brief Execute all loaded processes as concurrent threads
 * 
 * Creates one thread per process and waits for all threads to complete.
 * This demonstrates concurrent process execution where multiple processes
 * can run simultaneously.
 * 
 * Thread Management:
 * 1. Create all threads and store them in a vector
 * 2. Join all threads to wait for completion before returning
 * 
 * This ensures all processes complete before the function returns.
 */
void ProcessSimulator::executeProcesses() {
    std::vector<std::thread> threads;
    
    // Create a thread for each process
    // Each thread will execute the processWorker function
    for (const auto& process : processes) {
        threads.push_back(std::thread(processWorker, process.pid, process.burstTime, this));
    }
    
    // Wait for all threads to complete (join all threads)
    // This ensures all processes finish before we return
    for (auto& thread : threads) {
        thread.join();
    }
}
