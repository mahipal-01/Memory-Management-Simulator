#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstddef>

struct CacheLine {
    bool valid;
    size_t tag_bits;
    size_t last_access;

    CacheLine();
};

class CacheSimulator {
private:
    size_t cache_capacity;
    size_t block_size;
    size_t associativity;
    size_t set_count;
    size_t access_counter;
    std::vector<std::vector<CacheLine>> cache_sets;

public:
    size_t hits;
    size_t misses;

    CacheSimulator(size_t capacity = 0, size_t line_size = 1, size_t ways = 1);
    bool access_memory(size_t address);
};

#endif
