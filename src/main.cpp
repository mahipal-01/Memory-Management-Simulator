#include "simulator.h"
#include <iostream>
using namespace std;

int main() {
    MemoryAllocator allocator;
    string cmd;

    cout << "Memory Simulator\n";

    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "init") {
            size_t m; cin >> m;
            allocator.initialize_memory(m);
        }
        else if (cmd == "set") {
            string s; cin >> s;
            allocator.set_allocation_strategy(s);
        }
        else if (cmd == "malloc") {
            size_t s; cin >> s;
            allocator.request_allocation(s);
        }
        else if (cmd == "free") {
            int id; cin >> id;
            allocator.free_memory(id);
        }
        else if (cmd == "dump") allocator.display_memory_layout();
        else if (cmd == "stats") allocator.print_statistics();
        else if (cmd == "access") {
            size_t a; cin >> a;
            allocator.simulate_access(a);
        }
        else if (cmd == "exit") break;
    }
}
