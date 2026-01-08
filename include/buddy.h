#ifndef BUDDY_H
#define BUDDY_H

#include <map>
#include <set>
#include <cstddef>

class BuddyAllocator {
private:
    size_t maximum_memory;
    std::map<size_t, std::set<size_t>> free_blocks;

    size_t round_to_power_of_two(size_t value);

public:
    BuddyAllocator(size_t total_memory);
    long long allocate_block(size_t request_size, size_t& allocated_size);
    void free_block(size_t address, size_t block_size);
};

#endif
