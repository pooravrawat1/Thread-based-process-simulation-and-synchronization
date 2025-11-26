/**
 * @file main.cpp
 * @brief Main entry point for the Thread-Based Process Simulation System
 * 
 * This program demonstrates operating system synchronization concepts through
 * two simulations:
 * 1. Process Simulation: Simulates concurrent process execution using threads
 * 2. Dining Philosophers: Demonstrates deadlock prevention using ordered resource acquisition
 * 
 * The program reads process data from processes.txt and creates threads to simulate
 * concurrent process execution. It then runs the classic Dining Philosophers problem
 * with 5 philosophers and implements deadlock prevention through ordered resource
 * acquisition (always acquiring the lower-numbered fork first).
 * 
 * @author Thread Simulation System
 * @date 2024
 */

#include <iostream>
#include <iomanip>
#include "ProcessSimulator.h"
#include "DiningPhilosophers.h"

/**
 * @brief Main function - coordinates execution of both simulations
 * 
 * Executes the process simulation first, followed by the dining philosophers
 * simulation. Provides formatted output with section separators for clarity.
 * 
 * @return 0 on success, 1 if process loading fails
 */
int main() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  THREAD-BASED PROCESS SIMULATION SYSTEM" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    // Part 1: Process Simulation
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "  PART 1: PROCESS SIMULATION" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "  Loading processes from file and simulating execution..." << std::endl;
    std::cout << std::string(60, '-') << std::endl << std::endl;
    
    ProcessSimulator procSim;
    
    // Load processes from file - reads process ID and burst time pairs
    if (!procSim.loadProcesses("processes.txt")) {
        std::cerr << "\n[ERROR] Failed to load processes. Exiting." << std::endl;
        return 1;
    }
    
    // Execute all process threads - creates one thread per process
    // Each thread simulates CPU burst time using sleep
    procSim.executeProcesses();
    
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "  All processes completed successfully." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Section separator between simulations
    std::cout << "\n\n" << std::string(60, '=') << std::endl;
    
    // Part 2: Dining Philosophers Simulation
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "  PART 2: DINING PHILOSOPHERS SIMULATION" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "  Simulating 5 philosophers with deadlock prevention..." << std::endl;
    std::cout << "  Strategy: Ordered resource acquisition" << std::endl;
    std::cout << std::string(60, '-') << std::endl << std::endl;
    
    // Create dining philosophers simulation with 3 think-eat cycles per philosopher
    // Deadlock Prevention Strategy: Ordered resource acquisition
    // - Each philosopher picks up the lower-numbered fork first, then the higher-numbered fork
    // - This breaks the circular wait condition and prevents deadlock
    DiningPhilosophers philSim(3);
    philSim.simulate();
    
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "  All philosophers completed successfully." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  SIMULATION COMPLETE" << std::endl;
    std::cout << std::string(60, '=') << std::endl << std::endl;
    
    return 0;
}
