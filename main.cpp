#include <iostream>
#include "ProcessSimulator.h"

int main() {
    ProcessSimulator procSim;
    
    // Test loading processes from file
    if (!procSim.loadProcesses("processes.txt")) {
        std::cerr << "Failed to load processes. Exiting." << std::endl;
        return 1;
    }
    
    std::cout << "Processes loaded successfully!" << std::endl;
    
    return 0;
}
