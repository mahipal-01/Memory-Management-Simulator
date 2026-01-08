#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "block.h"
#include "cache.h"
#include "buddy.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
using namespace std;
class MemoryAllocator {
private:
    size_t total_memory;
    AllocationStrategy current_strategy;
    std::vector<MemoryBlock> memory_layout;
    int next_block_id;

    size_t allocation_requests;
    size_t successful_allocations;
    size_t failed_allocations;
    size_t buddy_allocated_bytes;
    size_t buddy_wasted_bytes;
    size_t l1_to_l2_misses;
    size_t l2_to_memory_misses;

    CacheSimulator level1_cache;
    CacheSimulator level2_cache;
    BuddyAllocator* buddy_system;
    unordered_map<int, pair<size_t, size_t>> buddy_allocation_table;


    void merge_adjacent_free_blocks();

public:
    MemoryAllocator();
    ~MemoryAllocator();

    void initialize_memory(size_t size);
    void set_allocation_strategy(const std::string& name);
    void request_allocation(size_t bytes);
    void free_memory(int block_id);
    void simulate_access(size_t address);
    void display_memory_layout();
    void print_statistics();
};

#endif
