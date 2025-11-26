/**
 * @file DiningPhilosophers.cpp
 * @brief Implementation of the DiningPhilosophers class
 * 
 * This file implements the classic Dining Philosophers synchronization problem
 * with deadlock prevention. Five philosophers sit at a round table with five forks.
 * Each philosopher alternates between thinking and eating, requiring two forks to eat.
 * 
 * DEADLOCK PREVENTION STRATEGY: ORDERED RESOURCE ACQUISITION
 * ============================================================
 * The key to preventing deadlock is ensuring philosophers always pick up forks
 * in a consistent order. Specifically:
 * 
 * 1. Each philosopher identifies their left and right fork numbers
 * 2. The philosopher picks up the LOWER-numbered fork first
 * 3. Then picks up the HIGHER-numbered fork second
 * 
 * Why this prevents deadlock:
 * - Deadlock requires circular wait (A waits for B, B waits for C, C waits for A)
 * - By ordering resource acquisition, we break the circular wait condition
 * - At least one philosopher will always be able to acquire both forks
 * 
 * Example:
 * - Philosopher 0: needs forks 0 and 4 → picks up 0 first, then 4
 * - Philosopher 1: needs forks 1 and 0 → picks up 0 first, then 1
 * - Philosopher 4: needs forks 4 and 3 → picks up 3 first, then 4
 * 
 * Thread Safety:
 * - Each fork is represented by a std::mutex (CRITICAL RESOURCE)
 * - Console output is protected by coutMutex (CRITICAL SECTION)
 * - Uses RAII pattern (std::lock_guard) for automatic mutex unlocking
 * 
 * @author Thread Simulation System
 * @date 2024
 */

#include "DiningPhilosophers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

/**
 * @brief Constructor - initializes the dining philosophers simulation
 * @param iterations Number of think-eat cycles per philosopher
 */
DiningPhilosophers::DiningPhilosophers(int iterations) : iterations(iterations) {
    // Initialize start time for timestamp tracking
    startTime = std::chrono::steady_clock::now();
    // Member variables are initialized:
    // - forks array (default constructed mutexes)
    // - coutMutex (default constructed)
    // - iterations (via initializer list)
}

/**
 * @brief Get the index of the left fork for a philosopher
 * 
 * Fork arrangement: Philosopher i has fork i on their left
 * 
 * @param id Philosopher ID (0-4)
 * @return Index of the left fork
 */
int DiningPhilosophers::leftFork(int id) {
    return id;
}

/**
 * @brief Get the index of the right fork for a philosopher
 * 
 * Fork arrangement: Philosopher i has fork (i+1) % 5 on their right
 * The modulo ensures philosopher 4's right fork is fork 0 (circular table)
 * 
 * @param id Philosopher ID (0-4)
 * @return Index of the right fork
 */
int DiningPhilosophers::rightFork(int id) {
    return (id + 1) % NUM_PHILOSOPHERS;
}

/**
 * @brief Get elapsed time since simulation start
 * 
 * Calculates the time elapsed since the DiningPhilosophers was created.
 * Used for timestamp logging to show the sequence of events.
 * 
 * @return Formatted timestamp string in seconds (e.g., "1.234")
 */
std::string DiningPhilosophers::getTimestamp() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    
    double seconds = elapsed.count() / 1000.0;
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << seconds;
    return oss.str();
}

/**
 * @brief Thread-safe logging with timestamp
 * 
 * CRITICAL SECTION: This method acquires coutMutex before writing to console.
 * This prevents interleaved output when multiple philosopher threads log simultaneously.
 * 
 * Uses std::lock_guard for RAII-style automatic mutex unlocking when the
 * function exits (even if an exception is thrown).
 * 
 * @param message The message to log to console
 */
void DiningPhilosophers::log(const std::string& message) {
    // CRITICAL SECTION BEGIN - acquire lock for thread-safe console output
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[" << getTimestamp() << "s] " << message << std::endl;
    // CRITICAL SECTION END - lock automatically released when lock_guard goes out of scope
}

/**
 * @brief Philosopher thinks for a random duration
 * 
 * Simulates the philosopher thinking by sleeping for a random duration
 * between 1 and 3 seconds. This represents the philosopher not needing
 * any resources (forks) during this time.
 * 
 * @param id Philosopher ID (0-4)
 */
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

/**
 * @brief Philosopher picks up both forks using ordered resource acquisition
 * 
 * DEADLOCK PREVENTION STRATEGY: ORDERED RESOURCE ACQUISITION
 * ===========================================================
 * This is the CRITICAL method that prevents deadlock. The key insight is:
 * 
 * 1. Identify which fork has the lower number and which has the higher number
 * 2. ALWAYS pick up the lower-numbered fork FIRST
 * 3. Then pick up the higher-numbered fork SECOND
 * 
 * Why this prevents deadlock:
 * - Without ordering, all philosophers could pick up their left fork simultaneously,
 *   then wait forever for their right fork (circular wait = deadlock)
 * - With ordering, at least one philosopher will always be able to get both forks
 * - This breaks the circular wait condition, one of the four necessary conditions for deadlock
 * 
 * Example scenario:
 * - Philosopher 0: needs forks 0 and 4 → picks up 0 first, then 4
 * - Philosopher 4: needs forks 4 and 3 → picks up 3 first, then 4
 * - Both want fork 4, but Philosopher 0 must get fork 0 first, and Philosopher 4
 *   must get fork 3 first. This ordering prevents circular wait.
 * 
 * CRITICAL SECTION: Acquires two fork mutexes (shared resources)
 * 
 * @param id Philosopher ID (0-4)
 */
void DiningPhilosophers::pickupForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    // DEADLOCK PREVENTION: Ordered resource acquisition
    // Always pick up lower-numbered fork first, then higher-numbered fork
    int firstFork = (left < right) ? left : right;
    int secondFork = (left < right) ? right : left;
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Waiting for forks " << left << " and " << right;
    log(ss.str());
    
    // CRITICAL SECTION BEGIN - acquire first fork (lower-numbered)
    forks[firstFork].lock();
    ss.str("");
    ss << "PHIL " << id << " | Acquired fork " << firstFork;
    log(ss.str());
    
    // CRITICAL SECTION CONTINUE - acquire second fork (higher-numbered)
    forks[secondFork].lock();
    ss.str("");
    ss << "PHIL " << id << " | Acquired fork " << secondFork;
    log(ss.str());
    // Both forks now held - philosopher can eat
}

/**
 * @brief Philosopher eats for a fixed duration
 * 
 * Simulates the philosopher eating by sleeping for 2 seconds.
 * During this time, the philosopher holds both forks (mutexes are locked).
 * 
 * @param id Philosopher ID (0-4)
 */
void DiningPhilosophers::eat(int id) {
    std::stringstream ss;
    ss << "PHIL " << id << " | Eating...";
    log(ss.str());
    
    // Eat for 2 seconds (holding both forks)
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

/**
 * @brief Philosopher puts down both forks
 * 
 * Releases both fork mutexes, making them available for other philosophers.
 * The order of release doesn't matter for correctness (unlike acquisition).
 * 
 * CRITICAL SECTION END: Releases two fork mutexes (shared resources)
 * 
 * @param id Philosopher ID (0-4)
 */
void DiningPhilosophers::putdownForks(int id) {
    int left = leftFork(id);
    int right = rightFork(id);
    
    // CRITICAL SECTION END - release both forks
    // Order of release doesn't matter (unlike acquisition order)
    forks[left].unlock();
    forks[right].unlock();
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Released forks " << left << " and " << right;
    log(ss.str());
}

/**
 * @brief Worker function executed by each philosopher thread
 * 
 * This static function is the entry point for each philosopher thread.
 * It performs multiple think-eat cycles based on the iterations parameter.
 * 
 * Each cycle consists of:
 * 1. Think (random duration, no resources needed)
 * 2. Pick up forks (acquire two mutexes using ordered acquisition)
 * 3. Eat (fixed duration, holding both forks)
 * 4. Put down forks (release both mutexes)
 * 
 * Thread Safety: All methods called use thread-safe logging
 * 
 * @param id Philosopher ID (0-4)
 * @param sim Pointer to DiningPhilosophers instance
 */
void DiningPhilosophers::philosopherWorker(int id, DiningPhilosophers* sim) {
    // Loop for multiple think-eat cycles based on iterations
    for (int i = 0; i < sim->iterations; i++) {
        std::stringstream ss;
        ss << "PHIL " << id << " | Starting cycle " << (i + 1) << " of " << sim->iterations;
        sim->log(ss.str());
        
        // Execute one think-eat cycle
        sim->think(id);           // Think (no resources needed)
        sim->pickupForks(id);     // Acquire forks (CRITICAL: ordered acquisition prevents deadlock)
        sim->eat(id);             // Eat (holding both forks)
        sim->putdownForks(id);    // Release forks (make available for others)
    }
    
    std::stringstream ss;
    ss << "PHIL " << id << " | Completed all " << sim->iterations << " iterations";
    sim->log(ss.str());
}

/**
 * @brief Start the dining philosophers simulation
 * 
 * Creates 5 philosopher threads and waits for all to complete.
 * Each philosopher will perform the specified number of think-eat cycles.
 * 
 * Thread Management:
 * 1. Create all philosopher threads and store them in an array
 * 2. Join all threads to wait for completion before returning
 * 
 * This ensures all philosophers complete their iterations before the function returns.
 */
void DiningPhilosophers::simulate() {
    // Create 5 philosopher threads
    std::thread philosophers[NUM_PHILOSOPHERS];
    
    // Create each philosopher thread
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = std::thread(philosopherWorker, i, this);
    }
    
    // Wait for all philosophers to complete (join all threads)
    // This ensures all philosophers finish before we return
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].join();
    }
}
