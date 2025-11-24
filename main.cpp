#include <iostream>
#include "ProcessSimulator.h"

int main() {
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
    
    return 0;
}
