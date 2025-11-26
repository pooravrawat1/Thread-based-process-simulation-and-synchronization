/**
 * @file ProcessSimulator.h
 * @brief Header file for the ProcessSimulator class
 * 
 * This file defines the ProcessSimulator class which manages concurrent process
 * execution simulation using C++ threads. Each process is represented as a thread
 * that simulates CPU burst time using sleep operations.
 * 
 * Thread Safety:
 * - All console output is protected by coutMutex to prevent interleaved messages
 * - Uses RAII pattern (std::lock_guard) for automatic mutex unlocking
 * 
 * @author Thread Simulation System
 * @date 2024
 */

#ifndef PROCESS_SIMULATOR_H
#define PROCESS_SIMULATOR_H

#include <vector>
#include <mutex>
#include <string>
#include <chrono>

/**
 * @struct Process
 * @brief Represents a process with an ID and CPU burst time
 */
struct Process {
    int pid;        ///< Process ID (must be positive)
    int burstTime;  ///< CPU burst time in seconds (must be positive)
};

/**
 * @class ProcessSimulator
 * @brief Manages process thread creation and execution
 * 
 * This class loads process data from a file and creates threads to simulate
 * concurrent process execution. Each process thread sleeps for its burst time
 * to simulate CPU usage.
 */
class ProcessSimulator {
private:
    std::vector<Process> processes;  ///< Vector storing all loaded processes
    std::mutex coutMutex;            ///< Mutex for thread-safe console output (CRITICAL SECTION)
    std::chrono::steady_clock::time_point startTime;  ///< Start time for timestamp calculation

public:
    /**
     * @brief Constructor - initializes start time for timestamp tracking
     */
    ProcessSimulator();
    
    /**
     * @brief Destructor - cleanup resources
     */
    ~ProcessSimulator();
    
    /**
     * @brief Load processes from a text file
     * 
     * Reads process data from the specified file. Each line should contain
     * a process ID followed by burst time (space-separated).
     * 
     * @param filename Path to the input file (e.g., "processes.txt")
     * @return true if file loaded successfully, false otherwise
     */
    bool loadProcesses(const std::string& filename);
    
    /**
     * @brief Execute all loaded processes as concurrent threads
     * 
     * Creates one thread per process and waits for all threads to complete.
     * Each thread simulates CPU burst time using sleep.
     */
    void executeProcesses();
    
    /**
     * @brief Thread-safe logging with timestamp
     * 
     * CRITICAL SECTION: Acquires coutMutex before writing to console
     * to prevent interleaved output from multiple threads.
     * 
     * @param message The message to log
     */
    void log(const std::string& message);
    
    /**
     * @brief Get elapsed time since simulation start
     * 
     * @return Formatted timestamp string in seconds
     */
    std::string getTimestamp();

private:
    /**
     * @brief Worker function executed by each process thread
     * 
     * This static function is the entry point for each process thread.
     * It logs the start, simulates CPU burst time, and logs completion.
     * 
     * @param pid Process ID
     * @param burstTime CPU burst time in seconds
     * @param sim Pointer to ProcessSimulator instance for logging
     */
    static void processWorker(int pid, int burstTime, ProcessSimulator* sim);
};

#endif // PROCESS_SIMULATOR_H
