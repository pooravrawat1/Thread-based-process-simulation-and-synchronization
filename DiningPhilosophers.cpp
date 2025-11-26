#include "DiningPhilosophers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

// Constructor with configurable iterations parameter
DiningPhilosophers::DiningPhilosophers(int iterations) : iterations(iterations) {
    // Initialize start time for timestamp tracking
    startTime = std::chrono::steady_clock::now();
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

// Get elapsed time since start
std::string DiningPhilosophers::getTimestamp() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    
    double seconds = elapsed.count() / 1000.0;
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << seconds;
    return oss.str();
}

// Thread-safe logging method
void DiningPhilosophers::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[" << getTimestamp() << "s] " << message << std::endl;
}

// Philosopher thinks for a random duration
void DiningPhilosophers::think(int id) {
    std::stringstream ss;
    ss << "PHIL " << id << " | Thinking...";
    log(ss.str());
    
    // Random sleep duration between 1 and 3 seconds
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);
    int thinkTime = dis(gen);
    
    std::this_thread::sleep_for(std::chrono::seconds(thinkTime));
}

// Philosopher picks up forks using ordered resource acquisition (deadlock prevention)
void DiningPhilosophers::pickupForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    // Ordered resource acquisition: pick up lower-numbered fork first
    int firstFork = (left < right) ? left : right;
    int secondFork = (left < right) ? right : left;
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Waiting for forks " << left << " and " << right;
    log(ss.str());
    
    // Acquire first fork
    forks[firstFork].lock();
    ss.str("");
    ss << "PHIL " << id << " | Acquired fork " << firstFork;
    log(ss.str());
    
    // Acquire second fork
    forks[secondFork].lock();
    ss.str("");
    ss << "PHIL " << id << " | Acquired fork " << secondFork;
    log(ss.str());
}

// Philosopher eats for a fixed duration
void DiningPhilosophers::eat(int id) {
    std::stringstream ss;
    ss << "PHIL " << id << " | Eating...";
    log(ss.str());
    
    // Eat for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

// Philosopher puts down both forks
void DiningPhilosophers::putdownForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    // Release both forks
    forks[left].unlock();
    forks[right].unlock();
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Released forks " << left << " and " << right;
    log(ss.str());
}

// Worker function for each philosopher thread
void DiningPhilosophers::philosopherWorker(int id, DiningPhilosophers* sim) {
    // Loop for multiple think-eat cycles based on iterations
    for (int i = 0; i < sim->iterations; i++) {
        std::stringstream ss;
        ss << "PHIL " << id << " | Starting cycle " << (i + 1) << " of " << sim->iterations;
        sim->log(ss.str());
        
        sim->think(id);
        sim->pickupForks(id);
        sim->eat(id);
        sim->putdownForks(id);
    }
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Completed all " << sim->iterations << " iterations";
    sim->log(ss.str());
}

// Start the simulation by creating 5 philosopher threads
void DiningPhilosophers::simulate() {
    // Create 5 philosopher threads
    std::thread philosophers[NUM_PHILOSOPHERS];
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = std::thread(philosopherWorker, i, this);
    }
    
    // Wait for all philosophers to complete
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].join();
    }
}
