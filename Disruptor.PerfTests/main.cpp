#include "stdafx.h"
#include <fstream>

#include "DisruptorRunner.h"
#include "ThreadPoolRunner.h"

std::uint32_t NUM_OF_THREADS = 500;
std::uint32_t TOTAL_OPERATIONS = 1500000;
std::uint32_t NUM_OF_ELEMENTS_IN_EVENT = 100;
std::uint32_t RING_BUFFER_SIZE = 4096;
std::ofstream latencyResultsFile("latency.txt", std::ios::out | std::ios::trunc);

void writeLatencyResultsToFile(std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs_)
{
    for (auto& array : *latencyVectorInUs_) {
        for (auto& latency : array) {
            latencyResultsFile << latency << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [thread|disruptor]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "thread") {
        std::cout << "Running with Thread Pool..." << std::endl;
        ThreadPoolRunner threadPoolRunner(NUM_OF_THREADS, TOTAL_OPERATIONS, NUM_OF_ELEMENTS_IN_EVENT);
        threadPoolRunner.run();
        writeLatencyResultsToFile(threadPoolRunner.getLatencyVector());
    } else if (mode == "disruptor") {
        std::cout << "Running with Disruptor..." << std::endl;
        DisruptorRunner disruptorRunner(RING_BUFFER_SIZE, TOTAL_OPERATIONS, NUM_OF_ELEMENTS_IN_EVENT);
        disruptorRunner.run();
        writeLatencyResultsToFile(disruptorRunner.getLatencyVector());
    } else {
        std::cerr << "Invalid mode: " << mode << std::endl;
        std::cerr << "Usage: " << argv[0] << " [thread|disruptor]" << std::endl;
        return 1;
    }

    return 0;
}
