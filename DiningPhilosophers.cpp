#include "DiningPhilosophers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>

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

// Philosopher thinks for a random duration
void DiningPhilosophers::think(int id) {
    std::stringstream ss;
    ss << "[Philosopher " << id << "] Thinking...";
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
    ss << "[Philosopher " << id << "] Waiting for forks " << left << " and " << right;
    log(ss.str());
    
    // Acquire first fork
    forks[firstFork].lock();
    ss.str("");
    ss << "[Philosopher " << id << "] Picked up fork " << firstFork;
    log(ss.str());
    
    // Acquire second fork
    forks[secondFork].lock();
    ss.str("");
    ss << "[Philosopher " << id << "] Picked up fork " << secondFork;
    log(ss.str());
}

// Philosopher eats for a fixed duration
void DiningPhilosophers::eat(int id) {
    std::stringstream ss;
    ss << "[Philosopher " << id << "] Eating...";
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
    ss << "[Philosopher " << id << "] Released forks " << left << " and " << right;
    log(ss.str());
}
