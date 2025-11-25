#ifndef DINING_PHILOSOPHERS_H
#define DINING_PHILOSOPHERS_H

#include <mutex>

class DiningPhilosophers {
private:
    static const int NUM_PHILOSOPHERS = 5;
    std::mutex forks[NUM_PHILOSOPHERS];
    std::mutex coutMutex;
    int iterations;
    
    // Helper methods to get fork indices
    int leftFork(int id);
    int rightFork(int id);
    
public:
    // Constructor with configurable iterations parameter
    DiningPhilosophers(int iterations = 3);
    
    // Start the simulation
    void simulate();
    
    // Thread-safe logging
    void log(const std::string& message);
    
private:
    // Worker function for each philosopher thread
    static void philosopherWorker(int id, DiningPhilosophers* sim);
    
    // Philosopher actions
    void think(int id);
    void pickupForks(int id);
    void eat(int id);
    void putdownForks(int id);
};

#endif // DINING_PHILOSOPHERS_H
