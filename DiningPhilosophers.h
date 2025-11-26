/**
 * @file DiningPhilosophers.h
 * @brief Header file for the DiningPhilosophers class
 * 
 * This file defines the DiningPhilosophers class which implements the classic
 * Dining Philosophers synchronization problem. Five philosophers sit at a round
 * table with five forks. Each philosopher needs two forks to eat.
 * 
 * Deadlock Prevention Strategy: ORDERED RESOURCE ACQUISITION
 * - Each philosopher picks up the lower-numbered fork first, then the higher-numbered fork
 * - This breaks the circular wait condition and prevents deadlock
 * - Example: Philosopher 0 needs forks 0 and 4, so picks up fork 0 first, then fork 4
 * 
 * Thread Safety:
 * - Each fork is represented by a std::mutex
 * - Console output is protected by coutMutex to prevent interleaved messages
 * - Uses RAII pattern (std::lock_guard) for automatic mutex unlocking
 * 
 * @author Thread Simulation System
 * @date 2024
 */

#ifndef DINING_PHILOSOPHERS_H
#define DINING_PHILOSOPHERS_H

#include <mutex>
#include <chrono>

/**
 * @class DiningPhilosophers
 * @brief Implements the Dining Philosophers synchronization problem
 * 
 * This class simulates 5 philosophers who alternate between thinking and eating.
 * Each philosopher needs two adjacent forks to eat. Deadlock is prevented using
 * ordered resource acquisition (always pick up lower-numbered fork first).
 */
class DiningPhilosophers {
private:
    static const int NUM_PHILOSOPHERS = 5;  ///< Number of philosophers (and forks)
    std::mutex forks[NUM_PHILOSOPHERS];     ///< Array of mutexes representing forks (CRITICAL RESOURCES)
    std::mutex coutMutex;                   ///< Mutex for thread-safe console output (CRITICAL SECTION)
    int iterations;                         ///< Number of think-eat cycles per philosopher
    std::chrono::steady_clock::time_point startTime;  ///< Start time for timestamp calculation
    
    /**
     * @brief Get the index of the left fork for a philosopher
     * @param id Philosopher ID (0-4)
     * @return Index of the left fork
     */
    int leftFork(int id);
    
    /**
     * @brief Get the index of the right fork for a philosopher
     * @param id Philosopher ID (0-4)
     * @return Index of the right fork
     */
    int rightFork(int id);
    
public:
    /**
     * @brief Constructor with configurable iterations parameter
     * @param iterations Number of think-eat cycles per philosopher (default: 3)
     */
    DiningPhilosophers(int iterations = 3);
    
    /**
     * @brief Start the dining philosophers simulation
     * 
     * Creates 5 philosopher threads and waits for all to complete.
     * Each philosopher will perform the specified number of think-eat cycles.
     */
    void simulate();
    
    /**
     * @brief Thread-safe logging with timestamp
     * 
     * CRITICAL SECTION: Acquires coutMutex before writing to console
     * to prevent interleaved output from multiple threads.
     * 
     * @param message The message to log
     */
    void log(const std::string& message);
    
    /**
     * @brief Get elapsed time since simulation start
     * @return Formatted timestamp string in seconds
     */
    std::string getTimestamp();
    
private:
    /**
     * @brief Worker function executed by each philosopher thread
     * 
     * This static function is the entry point for each philosopher thread.
     * It performs multiple think-eat cycles based on the iterations parameter.
     * 
     * @param id Philosopher ID (0-4)
     * @param sim Pointer to DiningPhilosophers instance
     */
    static void philosopherWorker(int id, DiningPhilosophers* sim);
    
    /**
     * @brief Philosopher thinks for a random duration (1-3 seconds)
     * @param id Philosopher ID
     */
    void think(int id);
    
    /**
     * @brief Philosopher picks up both forks using ordered resource acquisition
     * 
     * DEADLOCK PREVENTION: Always picks up lower-numbered fork first, then higher-numbered fork.
     * This breaks the circular wait condition and prevents deadlock.
     * 
     * CRITICAL SECTION: Acquires two fork mutexes in a specific order
     * 
     * @param id Philosopher ID
     */
    void pickupForks(int id);
    
    /**
     * @brief Philosopher eats for a fixed duration (2 seconds)
     * @param id Philosopher ID
     */
    void eat(int id);
    
    /**
     * @brief Philosopher puts down both forks
     * 
     * CRITICAL SECTION: Releases two fork mutexes
     * 
     * @param id Philosopher ID
     */
    void putdownForks(int id);
};

#endif // DINING_PHILOSOPHERS_H
