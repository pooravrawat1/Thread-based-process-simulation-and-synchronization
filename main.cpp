#include <iostream>
#include "ProcessSimulator.h"
#include "DiningPhilosophers.h"

int main() {
    // Part 1: Process Simulation
    std::cout << "=== Process Simulation ===" << std::endl;
    
    ProcessSimulator procSim;
    
    // Load processes from file
    if (!procSim.loadProcesses("processes.txt")) {
        std::cerr << "Failed to load processes. Exiting." << std::endl;
        return 1;
    }
    
    // Execute all process threads
    procSim.executeProcesses();
    
    std::cout << "\nAll processes completed." << std::endl;
    
    // Section separator between simulations
    std::cout << "\n========================================\n" << std::endl;
    
    // Part 2: Dining Philosophers Simulation
    std::cout << "=== Dining Philosophers Simulation ===" << std::endl;
    
    DiningPhilosophers philSim(3);  // 3 think-eat cycles per philosopher
    philSim.simulate();
    
    std::cout << "\nAll philosophers completed." << std::endl;
    
    return 0;
}
