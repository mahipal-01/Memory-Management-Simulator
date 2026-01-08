#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>

struct MemoryBlock {
    size_t start_address;
    size_t total_size;
    size_t requested_size;
    bool is_available;
    int block_identifier;
};

enum AllocationStrategy {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    BUDDY_ALLOCATION
};

#endif
