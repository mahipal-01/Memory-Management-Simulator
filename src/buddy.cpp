#include "buddy.h"
#include <algorithm>

BuddyAllocator::BuddyAllocator(size_t total_memory)
    : maximum_memory(total_memory) {
    free_blocks[total_memory].insert(0);
}

size_t BuddyAllocator::round_to_power_of_two(size_t value) {
    size_t p = 1;
    while (p < value) p <<= 1;
    return p;
}

long long BuddyAllocator::allocate_block(size_t request, size_t& allocated) {
    allocated = round_to_power_of_two(request);

    auto it = free_blocks.lower_bound(allocated);
    while (it != free_blocks.end()) {
        if (!it->second.empty()) {
            size_t size = it->first;
            size_t addr = *it->second.begin();
            it->second.erase(it->second.begin());

            while (size > allocated) {
                size >>= 1;
                free_blocks[size].insert(addr + size);
            }
            return addr;
        }
        ++it;
    }
    return -1;
}

void BuddyAllocator::free_block(size_t address, size_t size) {
    while (true) {
        size_t buddy = address ^ size;
        auto& set = free_blocks[size];

        if (set.count(buddy)) {
            set.erase(buddy);
            address = std::min(address, buddy);
            size <<= 1;
        } else {
            set.insert(address);
            break;
        }
    }
}
