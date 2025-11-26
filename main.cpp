#include <iostream>
#include <iomanip>
#include "ProcessSimulator.h"
#include "DiningPhilosophers.h"

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
    
    // Load processes from file
    if (!procSim.loadProcesses("processes.txt")) {
        std::cerr << "\n[ERROR] Failed to load processes. Exiting." << std::endl;
        return 1;
    }
    
    // Execute all process threads
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
    
    DiningPhilosophers philSim(3);  // 3 think-eat cycles per philosopher
    philSim.simulate();
    
    std::cout << "\n" << std::string(60, '-') << std::endl;
    std::cout << "  All philosophers completed successfully." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  SIMULATION COMPLETE" << std::endl;
    std::cout << std::string(60, '=') << std::endl << std::endl;
    
    return 0;
}
