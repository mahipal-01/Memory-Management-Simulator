#include "simulator.h"
#include <iostream>
using namespace std;


    MemoryAllocator::MemoryAllocator() {
        total_memory = 0;
        current_strategy = FIRST_FIT;
        next_block_id = 1;
        allocation_requests = 0;
        successful_allocations = 0;
        failed_allocations = 0;
        buddy_allocated_bytes = 0;
        buddy_wasted_bytes = 0;
        l1_to_l2_misses = 0;
        l2_to_memory_misses = 0;
        buddy_system = nullptr;
    }

    MemoryAllocator::~MemoryAllocator() {
        if (buddy_system != nullptr) {
            delete buddy_system;
        }
    }

    void MemoryAllocator::initialize_memory(size_t memory_size) {
        total_memory = memory_size;
        memory_layout.clear();
        
        MemoryBlock initial_block;
        initial_block.start_address = 0;
        initial_block.total_size = memory_size;
        initial_block.requested_size = 0;
        initial_block.is_available = true;
        initial_block.block_identifier = -1;
        memory_layout.push_back(initial_block);

        next_block_id = 1;
        allocation_requests = 0;
        successful_allocations = 0;
        failed_allocations = 0;
        buddy_allocated_bytes = 0;
        buddy_wasted_bytes = 0;
        l1_to_l2_misses = 0;
        l2_to_memory_misses = 0;

        level1_cache = CacheSimulator(64, 16, 2);
        level2_cache = CacheSimulator(256, 32, 4);

        if (buddy_system != nullptr) {
            delete buddy_system;
        }
        buddy_system = nullptr;
        buddy_allocation_table.clear();
    }

    void MemoryAllocator::set_allocation_strategy(const string& strategy_name) {
        if (strategy_name == "first_fit") {
            current_strategy = FIRST_FIT;
        } else if (strategy_name == "best_fit") {
            current_strategy = BEST_FIT;
        } else if (strategy_name == "worst_fit") {
            current_strategy = WORST_FIT;
        } else if (strategy_name == "buddy") {
            current_strategy = BUDDY_ALLOCATION;
            if (buddy_system != nullptr) {
                delete buddy_system;
            }
            buddy_system = new BuddyAllocator(total_memory);
        }
    }

    void MemoryAllocator::request_allocation(size_t byte_count) {
        allocation_requests++;

        if (current_strategy == BUDDY_ALLOCATION) {
            size_t actual_allocated;
            long long address = buddy_system->allocate_block(byte_count, actual_allocated);

            if (address < 0) {
                failed_allocations++;
                cout << "Allocation failed\n";
                return;
            }

            int identifier = next_block_id++;
            buddy_allocation_table[identifier] = make_pair(address, actual_allocated);
            buddy_allocated_bytes += actual_allocated;
            buddy_wasted_bytes += (actual_allocated - byte_count);
            successful_allocations++;

            cout << "Allocated block id=" << identifier << " at " << address << "\n";
            return;
        }

        // Find suitable block using selected strategy
        int selected_index = -1;
        for (int i = 0; i < (int)memory_layout.size(); i++) {
            if (!memory_layout[i].is_available || memory_layout[i].total_size < byte_count) {
                continue;
            }

            if (selected_index == -1) {
                selected_index = i;
            } else if (current_strategy == BEST_FIT && 
                       memory_layout[i].total_size < memory_layout[selected_index].total_size) {
                selected_index = i;
            } else if (current_strategy == WORST_FIT && 
                       memory_layout[i].total_size > memory_layout[selected_index].total_size) {
                selected_index = i;
            }

            if (current_strategy == FIRST_FIT) {
                break;
            }
        }

        if (selected_index == -1) {
            failed_allocations++;
            cout << "Allocation failed\n";
            return;
        }

        MemoryBlock previous_block = memory_layout[selected_index];
        memory_layout.erase(memory_layout.begin() + selected_index);

        MemoryBlock new_allocated;
        new_allocated.start_address = previous_block.start_address;
        new_allocated.total_size = byte_count;
        new_allocated.requested_size = byte_count;
        new_allocated.is_available = false;
        new_allocated.block_identifier = next_block_id++;
        memory_layout.insert(memory_layout.begin() + selected_index, new_allocated);

        if (previous_block.total_size > byte_count) {
            MemoryBlock remaining;
            remaining.start_address = previous_block.start_address + byte_count;
            remaining.total_size = previous_block.total_size - byte_count;
            remaining.requested_size = 0;
            remaining.is_available = true;
            remaining.block_identifier = -1;
            memory_layout.insert(memory_layout.begin() + selected_index + 1, remaining);
        }

        successful_allocations++;
        cout << "Allocated block id=" << (next_block_id - 1) 
             << " at " << previous_block.start_address << "\n";
    }

    void MemoryAllocator::free_memory(int block_id) {
        if (current_strategy == BUDDY_ALLOCATION) {
            if (buddy_allocation_table.find(block_id) == buddy_allocation_table.end()) {
                cout << "Invalid block id\n";
                return;
            }

            pair<size_t, size_t> block_info = buddy_allocation_table[block_id];
            buddy_system->free_block(block_info.first, block_info.second);
            buddy_allocated_bytes -= block_info.second;
            buddy_allocation_table.erase(block_id);

            cout << "Block " << block_id << " freed\n";
            return;
        }

        for (size_t i = 0; i < memory_layout.size(); i++) {
            if (!memory_layout[i].is_available && 
                memory_layout[i].block_identifier == block_id) {
                memory_layout[i].is_available = true;
                memory_layout[i].block_identifier = -1;
                merge_adjacent_free_blocks();
                cout << "Block " << block_id << " freed\n";
                return;
            }
        }
        
        cout << "Invalid block id\n";
    }

    void MemoryAllocator::merge_adjacent_free_blocks() {
        size_t i = 0;
        while (i + 1 < memory_layout.size()) {
            if (memory_layout[i].is_available && memory_layout[i + 1].is_available) {
                memory_layout[i].total_size += memory_layout[i + 1].total_size;
                memory_layout.erase(memory_layout.begin() + i + 1);
            } else {
                i++;
            }
        }
    }

    void MemoryAllocator::simulate_access(size_t address) {
        bool l1_hit = level1_cache.access_memory(address);
        if (!l1_hit) {
            l1_to_l2_misses++;
            bool l2_hit = level2_cache.access_memory(address);
            if (!l2_hit) {
                l2_to_memory_misses++;
            }
        }
    }

    void MemoryAllocator::display_memory_layout() {
        if (current_strategy == BUDDY_ALLOCATION) {
            cout << "[Buddy allocator active]\n";
            return;
        }

        for (size_t i = 0; i < memory_layout.size(); i++) {
            MemoryBlock& block = memory_layout[i];
            cout << "[" << block.start_address << " - " 
                 << (block.start_address + block.total_size - 1) << "] ";
            
            if (block.is_available) {
                cout << "FREE\n";
            } else {
                cout << "USED (id=" << block.block_identifier << ")\n";
            }
        }
    }

    void MemoryAllocator::print_statistics() {
        size_t used_memory = buddy_allocated_bytes;
        size_t internal_waste = buddy_wasted_bytes;
        size_t largest_free_block = 0;

        if (current_strategy != BUDDY_ALLOCATION) {
            for (size_t i = 0; i < memory_layout.size(); i++) {
                if (!memory_layout[i].is_available) {
                    used_memory += memory_layout[i].total_size;
                    internal_waste += (memory_layout[i].total_size - 
                                      memory_layout[i].requested_size);
                } else {
                    if (memory_layout[i].total_size > largest_free_block) {
                        largest_free_block = memory_layout[i].total_size;
                    }
                }
            }
        }

        size_t free_memory = total_memory - used_memory;
        double external_fragmentation = 0.0;
        
        if (free_memory > 0 && current_strategy != BUDDY_ALLOCATION) {
            external_fragmentation = ((double)(free_memory - largest_free_block) / 
                                     free_memory) * 100.0;
        }

        cout << "Total memory: " << total_memory << "\n";
        cout << "Used memory: " << used_memory << "\n";
        cout << "Free memory: " << free_memory << "\n";
        cout << "Memory utilization: " 
             << ((double)used_memory / total_memory * 100) << "%\n";
        cout << "Internal fragmentation: " << internal_waste << " bytes\n";
        cout << "External fragmentation: " << external_fragmentation << "%\n";
        
        double success_rate = 0.0;
        if (allocation_requests > 0) {
            success_rate = ((double)successful_allocations / allocation_requests) * 100;
        }
        cout << "Allocation success rate: " << success_rate << "%\n";

        cout << "L1 hits/misses: " << level1_cache.hits << "/" 
             << level1_cache.misses << "\n";
        cout << "L2 hits/misses: " << level2_cache.hits << "/" 
             << level2_cache.misses << "\n";
        cout << "L1 miss -> L2: " << l1_to_l2_misses << "\n";
        cout << "L2 miss -> Memory: " << l2_to_memory_misses << "\n";
    }
