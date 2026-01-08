CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = src/main.cpp src/cache.cpp src/buddy.cpp src/allocator.cpp
OUT = memory_simulator

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	del /Q $(OUT).exe 2>nul || rm -f $(OUT)
