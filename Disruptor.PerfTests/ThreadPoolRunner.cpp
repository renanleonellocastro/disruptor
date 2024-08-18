#include "Event.h"
#include "ProcessingSteps.h"
#include "ThreadPoolRunner.h"

ThreadPoolRunner::ThreadPoolRunner(std::uint32_t numberOfThreads, std::uint32_t totalOperations, std::uint32_t numOfElementsInEvent)
    : numberOfThreads_(numberOfThreads), totalOperations_(totalOperations), numOfElementsInEvent_(numOfElementsInEvent)
{
    latencyVectorInUs_ = std::make_shared<std::vector<std::vector<std::int64_t>>>();
    operationsPerThread_ = totalOperations_ / numberOfThreads_;
    latencyVectorInUs_->resize(numberOfThreads_);

    for (std::uint32_t i = 0; i < numberOfThreads_; ++i) {
        (*latencyVectorInUs_)[i].resize(operationsPerThread_);
    }
}

std::shared_ptr<std::vector<std::vector<std::int64_t>>> ThreadPoolRunner::getLatencyVector()
{
    return latencyVectorInUs_;
}

void ThreadPoolRunner::run()
{
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (std::uint32_t i = 0; i < numberOfThreads_; ++i) {
        std::cout << "Starting thread " << i + 1 << std::endl;
        threads.emplace_back(&ThreadPoolRunner::runAllTasks, this, i);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total elapsed time: " << duration.count() << " ms" << std::endl;
    std::cout << "Throughput: " << totalOperations_ / duration.count() * 1000 << " QPS" << std::endl;
    std::cout << "GLOBAL_COUNTER: " << ProcessingSteps::getInstance().getCounter() << std::endl;
}

void ThreadPoolRunner::runAllTasks(std::uint32_t latencyVectorIndex)
{
    for (std::uint32_t i = 0; i < operationsPerThread_; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        runOneTask();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        (*latencyVectorInUs_)[latencyVectorIndex][i] = duration;
    }
}

void ThreadPoolRunner::runOneTask()
{
    Event evt;
    initializeEvent(evt, numOfElementsInEvent_);
    ProcessingSteps::getInstance().runProcess1(evt);
    ProcessingSteps::getInstance().runProcess2(evt);
    ProcessingSteps::getInstance().runProcess3(evt);
    ProcessingSteps::getInstance().runProcess4(evt);
    ProcessingSteps::getInstance().runProcess5(evt);
    ProcessingSteps::getInstance().runProcess6(evt);
    ProcessingSteps::getInstance().runProcess7WithContention();
    ProcessingSteps::getInstance().runProcess8(evt);
    ProcessingSteps::getInstance().runProcess9(evt);
    ProcessingSteps::getInstance().runProcess10(evt);
    ProcessingSteps::getInstance().runProcess11(evt);
    ProcessingSteps::getInstance().runProcess12(evt);
    ProcessingSteps::getInstance().runProcess13(evt);
    ProcessingSteps::getInstance().runProcess14(evt);
    ProcessingSteps::getInstance().runProcess15(evt);
}
