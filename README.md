# Memory Management Simulator

A comprehensive operating system memory management simulator implemented in C++ that models dynamic memory allocation, multilevel caching, and system statistics tracking.

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

**Allocation Operations:**
- `malloc(size)` - Allocate memory block
- `free(address/block_id)` - Deallocate memory block
- Automatic block splitting when necessary
- Block coalescing to reduce fragmentation

### ✅ Feature 3: Allocation Interface & Memory Visualization
Command-line interface supporting:
- `init memory [size]` - Initialize memory pool
- `set allocator [first_fit|best_fit|worst_fit]` - Select allocation strategy
- `malloc [size]` - Request memory allocation
- `free [block_id]` - Release allocated block
- `dump memory` - Visualize memory layout
- `stats` - Display fragmentation and utilization statistics

**Example Usage:**
```
$ ./memsim
> init memory 1024
> set allocator first_fit
> malloc 100
Allocated block id=1 at address=0x0000
> malloc 200
Allocated block id=2 at address=0x0064
> free 1
Block 1 freed and merged
> dump memory
[0x0000 - 0x0063] FREE
[0x0064 - 0x012B] USED (id=2)
[0x012C - 0x03FF] FREE
> stats
Total memory: 1024
Used memory: 200
External fragmentation: 35%
```

### ✅ Feature 4: Metrics and Statistics
Comprehensive performance metrics:
- **Internal Fragmentation**: Wasted space within allocated blocks
- **External Fragmentation**: Fragmentation percentage of free space
- **Allocation Success/Failure Rate**: Tracking allocation failures
- **Memory Utilization**: Percentage of memory in use vs. total

### ✅ Feature 5: Buddy Allocation System
Power-of-two based memory allocation:
- Memory size must be a power of two
- Allocation automatically rounds up to nearest power of two
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
- **Associativity**: Direct-mapped or set-associative configurations

#### Cache Replacement Policy:
- **FIFO (First-Come, First-Served)**: Evicts oldest cache entries

#### Performance Tracking:
- Cache hits and misses per level
- Hit ratio calculation
- Miss penalty propagation to lower cache levels
- Memory access latency modeling

### ❌ Not Implemented

- **Feature 7: Virtual Memory Simulation** - Paging not implemented
- **Feature 8: Integration Between Components** - Virtual memory integration not implemented

## Project Architecture

```
Memory-Management-Simulator/
├── src/
│   ├── allocator/
│   │   ├── allocator.h
│   │   ├── first_fit.cpp
│   │   ├── best_fit.cpp
│   │   └── worst_fit.cpp
│   ├── buddy/
│   │   ├── buddy.h
│   │   └── buddy.cpp
│   ├── cache/
│   │   ├── cache.h
│   │   ├── l1_cache.cpp
│   │   └── l2_cache.cpp
│   ├── memory/
│   │   ├── memory.h
│   │   └── memory.cpp
│   └── main.cpp
├── include/
│   ├── allocator.h
│   ├── buddy.h
│   ├── cache.h
│   └── memory.h
├── tests/
│   ├── test_allocator.cpp
│   └── test_cache.cpp
├── docs/
│   └── design_document.md
├── Makefile
└── README.md
```

## Memory Allocation Strategy Details

### First Fit
- **Time Complexity**: O(n) where n is number of blocks
- **Space Efficiency**: Generally good but may fragment
- **Use Case**: Quick allocation, moderate fragmentation
- Implementation traverses free list until suitable block found

### Best Fit
- **Time Complexity**: O(n) with full list traversal
- **Space Efficiency**: Better than First Fit, reduces waste
- **Use Case**: Optimized memory utilization
- Implementation finds smallest block that satisfies request

### Worst Fit
- **Time Complexity**: O(n) with full list traversal
- **Space Efficiency**: May lead to fragmentation
- **Use Case**: Keeping larger free blocks available
- Implementation allocates from largest available block

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
Total Memory: 1024 (2^10)

Initial: [1024 FREE]

malloc(256): [256 USED][768 FREE]

malloc(256): [256 USED][256 USED][512 FREE]

free(first 256): [256 FREE][256 USED][512 FREE]
                 → After coalescing: [512 FREE][256 USED][256 FREE]
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
make test
```

## Usage Examples

### Basic Memory Allocation
```
> init memory 2048
Memory initialized: 2048 bytes

> set allocator best_fit
Allocator set to: BEST_FIT

> malloc 512
Allocated block id=1 at address=0x0000

> malloc 256
Allocated block id=2 at address=0x0200

> dump memory
[0x0000 - 0x01FF] USED (id=1, size=512)
[0x0200 - 0x02FF] USED (id=2, size=256)
[0x0300 - 0x07FF] FREE (size=1280)
```

### Fragmentation Analysis
```
> stats
=== Memory Statistics ===
Total Memory: 2048 bytes
Used Memory: 768 bytes
Free Memory: 1280 bytes
Memory Utilization: 37.5%
External Fragmentation: 15.2%
Number of Free Blocks: 1
Number of Used Blocks: 2
Allocation Success Rate: 100%
```

### Buddy Allocation
```
> set allocator buddy
Allocator set to: BUDDY

> init memory 256
Memory initialized with Buddy System: 256 bytes

> malloc 32
Allocated block id=1 at address=0x0000 (size=32)

> malloc 64
Allocated block id=2 at address=0x0040 (size=64)

> stats
Total Memory: 256 bytes
Used Memory: 96 bytes
Fragmentation: 4 blocks
```

### Cache Simulation
```
> cache init l1 8192 64 4
L1 Cache initialized: 8KB, block size 64B, 4-way associative

> cache init l2 65536 64 2
L2 Cache initialized: 64KB, block size 64B, 2-way associative

> cache access read 0x1000
L1 Hit (hot data)

> cache access read 0x4000
L1 Miss, L2 Hit

> cache stats
=== Cache Statistics ===
L1 Cache:
  Total Accesses: 3
  Hits: 1
  Misses: 2
  Hit Ratio: 33.33%

L2 Cache:
  Total Accesses: 2
  Hits: 1
  Misses: 1
  Hit Ratio: 50.00%
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
- **Direct-Mapped**: Each memory location maps to exactly one cache location
- **Set-Associative**: Memory locations can map to multiple locations within a set
- **Trade-off**: Set-associative reduces conflicts but increases complexity

## Limitations and Simplifications

1. **No Virtual Memory**: Simulation does not implement paging or virtual-to-physical address translation
2. **Single-Threaded**: No multi-threading or process isolation
3. **Symbolic Disk Access**: Disk latency is not realistically modeled
4. **Fixed Cache Block Size**: L1 and L2 share same block size for simplification
5. **FIFO Replacement Only**: Advanced policies (LRU, LFU) not implemented
6. **No Memory Protection**: No access control or permission checking

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
   - Fragmentation metrics comparison
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

## Future Enhancements

- [ ] Implement LRU and LFU cache replacement policies
- [ ] Add virtual memory simulation with paging
- [ ] Support L3 cache hierarchy
- [ ] Implement process isolation
- [ ] Add detailed performance profiling
- [ ] Create visual memory layout display
- [ ] Support for cache write policies (write-back, write-through)
- [ ] Implement TLB for virtual memory

## References and Resources

### Operating Systems Textbooks
- *Operating System Concepts* - Silberschatz, Galvin, Gagne
- *Modern Operating Systems* - Andrew Tanenbaum

### Online Learning Resources
- [Gate Smashers OS Playlist](https://youtube.com/playlist?list=PLxCzCOWd7aiGz9donHRrE9I3Mwn6XdP8p)
- [GeeksForGeeks Operating Systems](https://www.geeksforgeeks.org/operating-systems/)

## Evaluation Criteria

- ✅ **Correctness**: All implemented algorithms function correctly
- ✅ **Faithful Implementation**: Accurate modeling of allocation and cache behavior
- ✅ **Code Quality**: Modular structure with clear separation of concerns
- ✅ **Documentation**: Comprehensive code comments and design documentation
- ✅ **Performance Analysis**: Detailed metrics and statistics collection

## License

This project is provided as-is for educational purposes.

## Author

**Mahipal** - [GitHub Profile](https://github.com/mahipal-01)

---

**Last Updated**: January 2026

For questions, issues, or contributions, please refer to the GitHub repository or contact the author.
