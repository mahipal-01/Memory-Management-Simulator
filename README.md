# Memory Management Simulator

A comprehensive operating system memory management simulator implemented in C++ that models dynamic memory allocation, multilevel caching, and system statistics tracking.

## Demo Video drive link: https://drive.google.com/file/d/1Nk_oQ9Lz3eaa5OAiCxP_Jnf3opgF-KI-/view?usp=sharing

## Overview

This project simulates fundamental OS memory management responsibilities, including physical memory allocation, deallocation, and multilevel CPU cache behavior. The simulator provides accurate modeling of OS memory-management behavior using well-defined data structures and algorithms, emphasizing **algorithmic correctness**, **performance trade-offs**, and **systems-level design**.

## Project Objectives

- Understand dynamic memory allocation strategies and fragmentation patterns
- Simulate memory allocation and deallocation at runtime with multiple algorithms
- Implement and compare cache replacement policies across cache hierarchy
- Model CPU cache behavior with realistic hit/miss tracking
- Gain hands-on experience with OS-level abstractions using user-space code

## Implemented Features

### ✅ Feature 1: Physical Memory Simulation
- Simulates a configurable contiguous block of physical memory
- Memory is dynamically divided based on allocation requests
- Memory units represented as bytes for precise management
- Full memory block tracking and visualization


### ✅ Feature 2: Memory Allocation Strategies
Implements three fundamental allocation algorithms:
- **First Fit**: Allocates in the first available free block large enough
- **Best Fit**: Finds the smallest free block that fits the request
- **Worst Fit**: Allocates in the largest available free block
### Memory Allocation Strategy Details

#### First Fit
- **Time Complexity**: O(n) where n is number of blocks
- **Space Efficiency**: Generally good but may fragment
- **Use Case**: Quick allocation, moderate fragmentation
- Implementation traverses free list until suitable block found

#### Best Fit
- **Time Complexity**: O(n) with full list traversal
- **Space Efficiency**: Better than First Fit, reduces waste
- **Use Case**: Optimized memory utilization
- Implementation finds smallest block that satisfies request

#### Worst Fit
- **Time Complexity**: O(n) with full list traversal
- **Space Efficiency**: May lead to fragmentation
- **Use Case**: Keeping larger free blocks available
- Implementation allocates from largest available block

**Allocation Operations:**
- `malloc size` - Allocate memory block
- `free block_id` - Deallocate memory block
- Automatic block splitting when necessary
- Block coalescing to reduce fragmentation

### ✅ Feature 3: Allocation Interface & Memory Visualization
Command-line interface supporting:
- `init memory size` - Initialize memory pool
- `set allocator first_fit|best_fit|worst_fit` - Select allocation strategy
- `malloc size` - Request memory allocation
- `free block_id` - Release allocated block
- `dump` - Visualize memory layout
- `stats` - Display fragmentation and utilization statistics

**Example Usage:**
```
$ ./memsim
> init 1024
> set first_fit
> malloc 100
Allocated block id=1 at 0
> malloc 200
Allocated block id=2 at 100
> malloc 50
Allocated block id=3 at 300
> free 2
Block 2 freed
> dump
[0 - 99] USED (id=1)
[100 - 299] FREE
[300 - 349] USED (id=3)
[350 - 1023] FREE
> stats
Total memory: 1024
Used memory: 150
Free memory: 874
Memory utilization: 14.6484%
Internal fragmentation: 0 bytes
External fragmentation: 22.8833%
Allocation success rate: 100%
L1 hits/misses: 0/0
L2 hits/misses: 0/0
L1 miss -> L2: 0
L2 miss -> Memory: 0
```

### ✅ Feature 4: Metrics and Statistics
Comprehensive performance metrics:
- **Internal Fragmentation**: Wasted space within allocated blocks
- **External Fragmentation**: Fragmentation percentage of free space
- **Allocation Success/Failure Rate**: Tracking allocation failures
- **Memory Utilization**: Percentage of memory in use vs. total

### ✅ Feature 5: Buddy Allocation System
Power-of-two-based memory allocation:
- Allocation automatically rounds up to the nearest power of two
- Maintains free lists for each block size
- Supports recursive splitting of memory blocks
- Efficient buddy coalescing for defragmentation
- Address computation using XOR operations for buddy finding

### ✅ Feature 6: Multilevel Cache Simulation

#### Cache Hierarchy Implementation:
- **L1 Cache**: First-level cache with configurable parameters
- **L2 Cache**: Second-level cache with larger capacity

#### Cache Configuration:
- **Configurable size** per cache level
- **Block size**: Cache line size (configurable)
- **Associativity**: set-associative configurations

#### Cache Replacement Policy:
- **LRU (Least Recently Used)**: pops the least recently used

#### Performance Tracking:
- Cache hits and misses per level
- Hit ratio calculation
- Miss penalty propagation to lower cache levels
- Memory access latency modeling



## Project Architecture

```
Memory-Management-Simulator/
├── src/
│   ├── allocator.cpp
│   ├── buddy.cpp
│   ├── cache.cpp
│   └── main.cpp
├── include/
│   ├── allocator.h
│   ├── buddy.h
│   ├── cache.h
│   └── memory.h
├── tests/
│   ├── test_buddy.txt
│   ├── test_first_fit.txt
│   ├── test_wrost_fit.txt
│   ├── test_best_fit.txt
│   └── test_cache.txt
├──document.pdf
├── Makefile
└── README.md
```


## Buddy Allocation System

The Buddy Allocation system provides efficient memory management:

### Key Features:
- Automatic power-of-two sizing
- Recursive block splitting for flexibility
- Efficient buddy location using XOR: `buddy_address = address XOR block_size`
- Coalescing of adjacent buddy blocks to reduce fragmentation
- O(log n) operations for split/merge

### Example:
```
> init 1024
> set buddy
> malloc 100
Allocated block id=1 at 0
> malloc 200
Allocated block id=2 at 256
> malloc 300
Allocated block id=3 at 512
> stats
Total memory: 1024
Used memory: 896
Free memory: 128
Memory utilization: 87.5%
Internal fragmentation: 296 bytes
External fragmentation: 0%
Allocation success rate: 100%
L1 hits/misses: 0/0
L2 hits/misses: 0/0
L1 miss -> L2: 0
L2 miss -> Memory: 0
```

## Cache Simulation Details

### L1 Cache
- First-level cache closest to CPU
- Smaller size, faster access
- Typical sizes: 4KB-32KB
- FIFO replacement policy

### L2 Cache
- Second-level cache with larger capacity
- Slower than L1, faster than main memory
- Typical sizes: 128KB-512KB
- Absorbs misses from L1 cache

### Cache Operations:
```
CPU Request
    ↓
L1 Cache Lookup
    ├─ Hit: Return data, update statistics
    └─ Miss: Check L2 Cache
        ├─ Hit: Load into L1, return data
        └─ Miss: Load from memory into both L1 and L2
```

### Statistics Collected:
- **Hit Ratio**: (Hits / Total Accesses) × 100%
- **Miss Rate**: (Misses / Total Accesses) × 100%
- **Memory Access Count**: Total external memory accesses
- **Cache Efficiency**: Metric showing cache performance

## Building and Running

### Prerequisites
- C++ compiler (C++11 or later)
- Make
- Standard C++ library

### Compilation
```bash
make clean
make all
```

### Running the Simulator
```bash
./memsim
```

### Running Tests
```bash
make run_tests.sh
```

## Usage Examples

### Basic Memory Allocation
```
> init memory 2048
Memory initialized: 2048 bytes

> set allocator best_fit
Allocator set to: BEST_FIT

> malloc 512
Allocated block id=1 at address=0

> malloc 256
Allocated block id=2 at address=512

> dump
[0 - 511] USED (id=1)
[512 - 767] USED (id=2)
[768 - 2047] FREE
```

### Fragmentation Analysis
```
> stats
=== Memory Statistics ===
Total memory: 2048
Used memory: 768
Free memory: 1280
Memory utilization: 37.5%
Internal fragmentation: 0 bytes
External fragmentation: 0%
Allocation success rate: 100%
L1 hits/misses: 0/0
L2 hits/misses: 0/0
L1 miss -> L2: 0
L2 miss -> Memory: 0
```

### Buddy Allocation
```
> set allocator buddy
Allocator set to: BUDDY

> init memory 256
Memory initialized with Buddy System: 256 bytes

> malloc 33
Allocated block id=1 at address=0 (size=32)

> malloc 64
Allocated block id=2 at address=64 (size=64)

> stats
Total memory: 256
Used memory: 128
Free memory: 128
Memory utilization: 50%
Internal fragmentation: 31 bytes
External fragmentation: 0%
Allocation success rate: 100%
L1 hits/misses: 0/0
L2 hits/misses: 0/0
L1 miss -> L2: 0
L2 miss -> Memory: 0
```

### Cache Simulation
```
init 512
> access 10
> access 10
> access 64
> access 64
> access 128
> stats

> cache stats
=== Cache Statistics ===
Total memory: 512
Used memory: 0
Free memory: 512
Memory utilization: 0%
Internal fragmentation: 0 bytes
External fragmentation: 0%
Allocation success rate: 0%
L1 hits/misses: 2/3
L2 hits/misses: 0/3
L1 miss -> L2: 3
L2 miss -> Memory: 3
```

## Design Decisions

### Memory Block Representation
- **Structure**: Linked list of memory blocks
- **Each Block Tracks**: Start address, size, allocation status, block ID
- **Advantages**: Efficient insertion/deletion, simple coalescing

### Fragmentation Metrics
- **External Fragmentation**: (Largest Free Block / Total Free Memory) × 100%
- **Calculation**: Tracks free block distribution to assess memory efficiency

### Cache Associativity
- **Set-Associative**: Memory locations can map to multiple locations within a set


## Performance Characteristics

### Time Complexity
| Operation | First Fit | Best Fit | Worst Fit | Buddy |
|-----------|-----------|----------|-----------|-------|
| malloc    | O(n)      | O(n)     | O(n)      | O(log n) |
| free      | O(n)      | O(n)     | O(n)      | O(log n) |
| coalesce  | O(n)      | O(n)     | O(n)      | O(log n) |

### Space Complexity
| Allocator | Space Required |
|-----------|-----------------|
| First Fit | O(n) blocks     |
| Best Fit  | O(n) blocks     |
| Worst Fit | O(n) blocks     |
| Buddy     | O(log n) lists  |

## Testing and Validation

### Test Categories

1. **Allocation Tests**
   - Single allocation and deallocation
   - Multiple sequential allocations
   - Fragmentation patterns
   - Coalescing verification

2. **Algorithm Comparison**
   - First Fit vs Best Fit vs Worst Fit performance
   - Allocation failure scenarios

3. **Buddy System Tests**
   - Power-of-two validation
   - Buddy address calculation
   - Split and merge operations
   - Boundary conditions

4. **Cache Tests**
   - L1 and L2 hit/miss tracking
   - Cache replacement policy validation
   - Multi-level access patterns
   - Performance metric accuracy



## References and Resources

### Operating Systems Textbooks
- *Operating System Concepts* - Silberschatz, Galvin, Gagne
- *Modern Operating Systems* - Andrew Tanenbaum

### Online Learning Resources
- [Gate Smashers OS Playlist](https://youtube.com/playlist?list=PLxCzCOWd7aiGz9donHRrE9I3Mwn6XdP8p)
- [GeeksForGeeks Operating Systems](https://www.geeksforgeeks.org/operating-systems/)



## License

This project is provided as-is for educational purposes.

## Author

**Mahipal** - [GitHub Profile](https://github.com/mahipal-01)

---

**Last Updated**: January 2026

For questions, issues, or contributions, please refer to the GitHub repository or contact the author.
