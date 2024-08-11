#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

#include <Disruptor/Disruptor.h>
#include <Disruptor/RingBuffer.h>
#include <Disruptor/ProducerType.h>
#include <Disruptor/IEventHandler.h>
#include <Disruptor/BusySpinWaitStrategy.h>
#include <Disruptor/ThreadPerTaskScheduler.h>

std::uint64_t GLOBAL_COUNTER = 0;
std::uint32_t NUM_OF_THREADS = 500;
std::uint32_t NUM_OF_OPERATIONS_PER_THREAD = 1000;
std::uint32_t TOTAL_OPERATIONS = NUM_OF_THREADS * NUM_OF_OPERATIONS_PER_THREAD;
std::mutex GLOBAL_MUTEX;

struct Event
{
    std::vector<int> data;
};

void initializeEvent(Event& evt, size_t size)
{
    evt.data.resize(size);

    for (int i = 0; i < size; ++i) {
        evt.data[i] = i;
    }
}

void step1(Event& evt)
{
    std::transform(evt.data.begin(), evt.data.end(), evt.data.begin(), [](int val) { return val * 2; });
}

void step2(Event& evt)
{
    std::for_each(evt.data.begin(), evt.data.end(), [](int& val) { val += 100; });
}

void step3(Event& evt)
{
    std::sort(evt.data.begin(), evt.data.end());
}

void step4(Event& evt)
{
    if (evt.data.empty()) return;

    // Compute prefix sum in-place
    for (size_t i = 1; i < evt.data.size(); ++i)
    {
        evt.data[i] += evt.data[i - 1];
    }
}

void step5(Event& evt)
{
    GLOBAL_MUTEX.lock();
    GLOBAL_COUNTER++;

    uint64_t cumulativeSum = 0;
    for (size_t i = 0; i < evt.data.size(); ++i)
    {
        cumulativeSum += evt.data[i];
        evt.data[i] = cumulativeSum / (i + 1); // Update with the running average
    }

    GLOBAL_MUTEX.unlock();
}


class Step1Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        step1(evt);
    }
};

class Step2Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        step2(evt);
    }
};

class Step3Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        step3(evt);
    }
};

class Step4Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        step4(evt);
    }
};

class Step5Handler : public Disruptor::IEventHandler<Event>
{
public:
    Step5Handler() : running(true)
    {
    }

    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        GLOBAL_COUNTER++;

        uint64_t cumulativeSum = 0;
        for (size_t i = 0; i < evt.data.size(); ++i)
        {
            cumulativeSum += evt.data[i];
            evt.data[i] = cumulativeSum / (i + 1); // Update with the running average
        }

        if (GLOBAL_COUNTER == TOTAL_OPERATIONS) {
            running = false;
        }
    }

    void waitEndOfProcessing()
    {
        while (running) {

        }
    }

private:
    bool running;
};

void threadFunction()
{
    for (std::uint32_t i = 0; i < NUM_OF_OPERATIONS_PER_THREAD; ++i)
    {
        Event evt;
        initializeEvent(evt, 10000);
        step1(evt);
        step2(evt);
        step3(evt);
        step4(evt);
        step5(evt);
    }
}

void runThreads()
{
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (std::uint32_t i = 0; i < NUM_OF_THREADS; ++i)
    {
        std::cout << "Starting thread " << i + 1 << std::endl;
        threads.emplace_back(threadFunction);
    }
    
    for (auto& th : threads)
    {
        th.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Total elapsed time: " << duration.count() << " ms" << std::endl;
    std::cout << "GLOBAL_COUNTER: " << GLOBAL_COUNTER << std::endl;
}

void runDisruptorPublisher(std::shared_ptr<Disruptor::disruptor<Event>> disruptor)
{
    for (std::uint32_t j = 0; j < TOTAL_OPERATIONS; ++j) {
        auto nextSequence = disruptor->ringBuffer()->next();  // Reserve the next slot in the ring buffer
        initializeEvent((*disruptor->ringBuffer())[nextSequence], 10000);
        disruptor->ringBuffer()->publish(nextSequence);  // Publish the event to make it available to the consumers
    }
}

void runDisruptor()
{
    auto const ringBufferSize = 1024;
    auto eventFactory = []() { return Event(); };
    auto taskScheduler = std::make_shared<Disruptor::ThreadPerTaskScheduler>();
    auto waitStrategy = std::make_shared<Disruptor::BusySpinWaitStrategy>();
    auto disruptor = std::make_shared<Disruptor::disruptor<Event>>(eventFactory, ringBufferSize, taskScheduler, Disruptor::ProducerType::Single, waitStrategy);

    auto step1Handler = std::make_shared<Step1Handler>();
    auto step2Handler = std::make_shared<Step2Handler>();
    auto step3Handler = std::make_shared<Step3Handler>();
    auto step4Handler = std::make_shared<Step4Handler>();
    auto step5Handler = std::make_shared<Step5Handler>();

    disruptor->handleEventsWith(step1Handler)
        ->then(step2Handler)
        ->then(step3Handler)
        ->then(step4Handler)
        ->then(step5Handler);

    taskScheduler->start();
    disruptor->start();

    auto start = std::chrono::high_resolution_clock::now();

    runDisruptorPublisher(disruptor);

    step5Handler->waitEndOfProcessing();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Total elapsed time: " << duration.count() << " ms" << std::endl;
    std::cout << "GLOBAL_COUNTER: " << GLOBAL_COUNTER << std::endl;

    disruptor->shutdown();
    taskScheduler->stop();
}

int main(int, char**)
{
    //runThreads();
    runDisruptor();
    return 0;
}
