# Thread-Based Process Simulation System

A C++ implementation demonstrating operating system synchronization concepts through two classic simulations: concurrent process execution and the Dining Philosophers problem with deadlock prevention.

## Overview

This project implements two key operating system concepts:

1. **Process Simulation**: Simulates concurrent process execution using threads, where each process has a CPU burst time
2. **Dining Philosophers**: Demonstrates the classic synchronization problem with deadlock prevention using ordered resource acquisition

## Features

- Multi-threaded process simulation with configurable burst times
- Deadlock-free implementation of the Dining Philosophers problem
- Thread-safe console logging with timestamps
- Comprehensive code documentation
- Input validation and error handling

## Requirements

### System Requirements
- **Operating System**: Windows, Linux, or macOS
- **C++ Compiler**: G++ with C++11 support or later
- **Threading Support**: POSIX threads (pthread)

### Windows Installation (MinGW-w64)
```powershell
# Install MSYS2 using winget
winget install -e --id MSYS2.MSYS2

# Open MSYS2 UCRT64 terminal and install GCC
pacman -S mingw-w64-ucrt-x86_64-gcc

# Add to PATH: C:\msys64\ucrt64\bin
```

### Linux Installation
```bash
# Ubuntu/Debian
sudo apt-get install g++

# Fedora/RHEL
sudo dnf install gcc-c++
```

### macOS Installation
```bash
# Using Homebrew
brew install gcc
```

## Project Structure

```
.
├── main.cpp                    # Main entry point
├── ProcessSimulator.h          # Process simulator header
├── ProcessSimulator.cpp        # Process simulator implementation
├── DiningPhilosophers.h        # Dining philosophers header
├── DiningPhilosophers.cpp      # Dining philosophers implementation
├── processes.txt               # Input file with process data
└── README.md                   # This file
```

## Building the Project

### Compilation Command
```bash
g++ -std=c++11 -pthread -o process_sim main.cpp ProcessSimulator.cpp DiningPhilosophers.cpp
```

### Compiler Flags Explained
- `-std=c++11`: Use C++11 standard (required for thread support)
- `-pthread`: Enable POSIX thread support
- `-o process_sim`: Output executable name

### Windows (PowerShell)
```powershell
g++ -std=c++11 -pthread -o process_sim.exe main.cpp ProcessSimulator.cpp DiningPhilosophers.cpp
```

## Running the Program

### Linux/macOS
```bash
./process_sim
```

### Windows
```powershell
.\process_sim.exe
```

## Input File Format

The `processes.txt` file contains process data in the following format:

```
<process_id> <burst_time>
```

### Example (processes.txt)
```
1 3
2 5
3 2
4 4
5 1
```

### Rules
- Each line contains two space-separated integers
- Process ID must be positive
- Burst time must be positive (in seconds)
- Empty lines are ignored

## Example Output

```
============================================================
  THREAD-BASED PROCESS SIMULATION SYSTEM
============================================================

------------------------------------------------------------
  PART 1: PROCESS SIMULATION
------------------------------------------------------------
  Loading processes from file and simulating execution...
------------------------------------------------------------

[0.016s] PROCESS   4 | Started (burst time: 4s)
[0.017s] PROCESS   1 | Started (burst time: 3s)
[0.017s] PROCESS   3 | Started (burst time: 2s)
[0.018s] PROCESS   2 | Started (burst time: 5s)
[0.019s] PROCESS   5 | Started (burst time: 1s)
[1.024s] PROCESS   5 | Finished
[2.024s] PROCESS   3 | Finished
[3.025s] PROCESS   1 | Finished
[4.023s] PROCESS   4 | Finished
[5.024s] PROCESS   2 | Finished

------------------------------------------------------------
  All processes completed successfully.
------------------------------------------------------------


============================================================

------------------------------------------------------------
  PART 2: DINING PHILOSOPHERS SIMULATION
------------------------------------------------------------
  Simulating 5 philosophers with deadlock prevention...
  Strategy: Ordered resource acquisition
------------------------------------------------------------

[0.011s] PHIL 0 | Starting cycle 1 of 3
[0.012s] PHIL 0 | Thinking...
[0.012s] PHIL 3 | Starting cycle 1 of 3
[0.013s] PHIL 3 | Thinking...
[0.013s] PHIL 1 | Starting cycle 1 of 3
[0.014s] PHIL 1 | Thinking...
[0.015s] PHIL 4 | Starting cycle 1 of 3
[0.015s] PHIL 2 | Starting cycle 1 of 3
[0.016s] PHIL 2 | Thinking...
[0.017s] PHIL 4 | Thinking...
[1.025s] PHIL 1 | Waiting for forks 1 and 2
[1.027s] PHIL 1 | Acquired fork 1
[1.028s] PHIL 1 | Acquired fork 2
[1.029s] PHIL 1 | Eating...
...
[18.118s] PHIL 2 | Released forks 2 and 3
[18.118s] PHIL 2 | Completed all 3 iterations

------------------------------------------------------------
  All philosophers completed successfully.
------------------------------------------------------------

============================================================
  SIMULATION COMPLETE
============================================================
```

## How It Works

### Part 1: Process Simulation

1. **Loading**: Reads process data from `processes.txt`
2. **Thread Creation**: Creates one thread per process
3. **Execution**: Each thread simulates CPU burst time using `sleep`
4. **Synchronization**: Uses mutex for thread-safe console output
5. **Completion**: Waits for all threads to finish before proceeding

### Part 2: Dining Philosophers

#### The Problem
- 5 philosophers sit at a round table with 5 forks
- Each philosopher needs 2 adjacent forks to eat
- Philosophers alternate between thinking and eating
- Without proper synchronization, deadlock can occur

#### Deadlock Prevention Strategy: Ordered Resource Acquisition

**The Solution**: Always acquire the lower-numbered fork first, then the higher-numbered fork.

**Why This Works**:
- Deadlock requires circular wait (A waits for B, B waits for C, C waits for A)
- By ordering resource acquisition, we break the circular wait condition
- At least one philosopher will always be able to acquire both forks

**Example**:
- Philosopher 0: needs forks 0 and 4 → picks up fork 0 first, then fork 4
- Philosopher 4: needs forks 4 and 3 → picks up fork 3 first, then fork 4
- Both want fork 4, but the ordering prevents circular wait

#### Implementation Details
- Each fork is represented by a `std::mutex`
- Philosophers think for random duration (1-3 seconds)
- Philosophers eat for fixed duration (2 seconds)
- Each philosopher completes 3 think-eat cycles
- All console output is thread-safe using a separate mutex

## Code Structure

### ProcessSimulator Class
- **loadProcesses()**: Reads and validates process data from file
- **executeProcesses()**: Creates and manages process threads
- **processWorker()**: Thread function that simulates process execution
- **log()**: Thread-safe logging with timestamps

### DiningPhilosophers Class
- **simulate()**: Creates and manages philosopher threads
- **philosopherWorker()**: Thread function for philosopher behavior
- **think()**: Simulates thinking (random sleep)
- **pickupForks()**: Acquires forks using ordered acquisition (deadlock prevention)
- **eat()**: Simulates eating (fixed sleep)
- **putdownForks()**: Releases both forks
- **log()**: Thread-safe logging with timestamps

## Customization

### Modify Number of Think-Eat Cycles
In `main.cpp`, change the parameter to `DiningPhilosophers` constructor:
```cpp
DiningPhilosophers philSim(5);  // 5 cycles instead of 3
```

### Modify Process Data
Edit `processes.txt` to add, remove, or modify processes:
```
1 2
2 4
3 1
```

### Adjust Timing
- **Think time**: Modify the range in `DiningPhilosophers::think()` (currently 1-3 seconds)
- **Eat time**: Modify the duration in `DiningPhilosophers::eat()` (currently 2 seconds)

## Thread Safety

All shared resources are protected by mutexes:

1. **Console Output**: Protected by `coutMutex` in both classes
2. **Forks**: Each fork is a `std::mutex` in the DiningPhilosophers class
3. **RAII Pattern**: Uses `std::lock_guard` for automatic mutex unlocking

## Error Handling

The program handles various error conditions:
- Missing or unreadable `processes.txt` file
- Invalid data format in input file
- Invalid process IDs (must be positive)
- Invalid burst times (must be positive)

## Verification

To verify the implementation works correctly:

1. **No Deadlocks**: Run the program multiple times - all philosophers should always complete
2. **Concurrent Execution**: Check timestamps - processes should start nearly simultaneously
3. **Thread Safety**: Log messages should never be interleaved or corrupted
4. **Correct Completion**: All processes and philosophers should complete successfully

## Technical Details

- **Language**: C++11
- **Threading**: POSIX threads (pthread)
- **Synchronization**: std::mutex, std::lock_guard
- **Timing**: std::chrono for timestamps and sleep
- **Compiler Version**: Tested with G++ 15.2.0
