#include "DiningPhilosophers.h"
#include <iostream>

// Constructor with configurable iterations parameter
DiningPhilosophers::DiningPhilosophers(int iterations) : iterations(iterations) {
    // Member variables are initialized:
    // - forks array (default constructed mutexes)
    // - coutMutex (default constructed)
    // - iterations (via initializer list)
}

// Helper method to get left fork index
int DiningPhilosophers::leftFork(int id) {
    return id;
}

// Helper method to get right fork index
int DiningPhilosophers::rightFork(int id) {
    return (id + 1) % NUM_PHILOSOPHERS;
}

// Thread-safe logging method
void DiningPhilosophers::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message << std::endl;
}
