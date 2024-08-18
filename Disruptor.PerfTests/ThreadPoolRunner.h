#ifndef THREADPOOLRUNNER_H
#define THREADPOOLRUNNER_H

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

class ThreadPoolRunner
{
public:
    ThreadPoolRunner(std::uint32_t numberOfThreads, std::uint32_t totalOperations, std::uint32_t numOfElementsInEvent);
    void run();
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> getLatencyVector();

private:
    void runAllTasks(std::uint32_t latencyVectorIndex);
    void runOneTask();

private:
    std::uint32_t numberOfThreads_;
    std::uint32_t totalOperations_;
    std::uint32_t operationsPerThread_;
    std::uint32_t numOfElementsInEvent_;
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs_;
};

#endif // THREADPOOLRUNNER_H
