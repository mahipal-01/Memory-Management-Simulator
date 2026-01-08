#!/bin/bash
make
./memory_simulator < tests/test_buddy.txt
./memory_simulator < tests/test_cache.txt
./memory_simulator < tests/test_first_fit.txt
./memory_simulator < tests/test_worst_fit.txt
./memory_simulator < tests/test_best_fit.txt


