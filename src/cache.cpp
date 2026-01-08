#include "cache.h"
#include <algorithm>

CacheLine::CacheLine() : valid(false), tag_bits(0), last_access(0) {}

CacheSimulator::CacheSimulator(size_t capacity, size_t line_size, size_t ways)
    : cache_capacity(capacity),
      block_size(line_size),
      associativity(ways),
      access_counter(0),
      hits(0),
      misses(0) {

    if (block_size == 0 || associativity == 0) return;

    size_t total_lines = cache_capacity / block_size;
    set_count = std::max((size_t)1, total_lines / associativity);
    cache_sets.resize(set_count, std::vector<CacheLine>(associativity));
}

bool CacheSimulator::access_memory(size_t address) {
    if (cache_sets.empty()) return false;

    access_counter++;
    size_t block = address / block_size;
    size_t set = block % set_count;
    size_t tag = block / set_count;

    auto& lines = cache_sets[set];

    for (auto& line : lines) {
        if (line.valid && line.tag_bits == tag) {
            hits++;
            line.last_access = access_counter;
            return true;
        }
    }

    misses++;
    CacheLine* victim = &lines[0];

    for (auto& line : lines) {
        if (!line.valid) {
            victim = &line;
            break;
        }
        if (line.last_access < victim->last_access)
            victim = &line;
    }

    victim->valid = true;
    victim->tag_bits = tag;
    victim->last_access = access_counter;
    return false;
}
