#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>

#include <Disruptor/Disruptor.h>
#include <Disruptor/RingBuffer.h>
#include <Disruptor/ProducerType.h>
#include <Disruptor/IEventHandler.h>
#include <Disruptor/BusySpinWaitStrategy.h>
#include <Disruptor/ThreadPerTaskScheduler.h>

std::uint64_t GLOBAL_COUNTER = 0;
std::uint32_t NUM_OF_THREADS = 500;
std::uint32_t TOTAL_OPERATIONS = 1500000;
std::uint32_t NUM_OF_OPERATIONS_PER_THREAD = TOTAL_OPERATIONS / NUM_OF_THREADS;
std::uint32_t NUM_OF_ELEMENTS_IN_EVENT = 100;
std::uint32_t RING_BUFFER_SIZE = 4096;
std::mutex GLOBAL_MUTEX;
std::ofstream file("/tmp/log.txt", std::ios::out | std::ios::trunc);

struct Event
{
    std::vector<int> data;
};

void initializeEvent(Event& evt)
{
    evt.data.resize(NUM_OF_ELEMENTS_IN_EVENT);

    for (int i = 0; i < NUM_OF_ELEMENTS_IN_EVENT; ++i) {
        evt.data[i] = i;
    }
}

void sequence1(Event& evt)
{
    std::transform(evt.data.begin(), evt.data.end(), evt.data.begin(), [](int val) { return val * 2; });
}

void sequence2(Event& evt)
{
    std::for_each(evt.data.begin(), evt.data.end(), [](int& val) { val += 100; });
}

void sequence3(Event& evt)
{
    std::sort(evt.data.begin(), evt.data.end());
}

void sequence4(Event& evt)
{
    if (evt.data.empty()) return;

    // Compute prefix sum in-place
    for (size_t i = 1; i < evt.data.size(); ++i)
    {
        evt.data[i] += evt.data[i - 1];
    }
}

void sequence5(Event& evt) { sequence1(evt); }

void sequence6(Event& evt) { sequence1(evt); }

void sequence7(Event& evt)
{
    GLOBAL_MUTEX.lock();

    GLOBAL_COUNTER++;
    file << "GLOBAL COUNTER: " << GLOBAL_COUNTER << std::endl;
    file.flush();

    GLOBAL_MUTEX.unlock();
}

void sequence8(Event& evt) { sequence1(evt); }

void sequence9(Event& evt) { sequence1(evt); }

void sequence10(Event& evt) { sequence1(evt); }

void sequence11(Event& evt) { sequence1(evt); }

void sequence12(Event& evt) { sequence1(evt); }

void sequence13(Event& evt) { sequence1(evt); }

void sequence14(Event& evt) { sequence1(evt); }

void sequence15(Event& evt) { sequence1(evt); }


class Sequence1Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence1(evt);
    }
};

class Sequence2Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence2(evt);
    }
};

class Sequence3Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence3(evt);
    }
};

class Sequence4Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence4(evt);
    }
};

class Sequence5Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence5(evt);
    }
};

class Sequence6Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence6(evt);
    }
};

class Sequence7Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence1(evt);
    }
};

class Sequence8Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence8(evt);
    }
};

class Sequence9Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence9(evt);
    }
};

class Sequence10Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence10(evt);
    }
};

class Sequence11Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence11(evt);
    }
};

class Sequence12Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence12(evt);
    }
};

class Sequence13Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence13(evt);
    }
};

class Sequence14Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        sequence14(evt);
    }
};

class Sequence15Handler : public Disruptor::IEventHandler<Event>
{
public:
    Sequence15Handler() : running(true)
    {
    }

    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override
    {
        GLOBAL_COUNTER++;

        file << "GLOBAL COUNTER: " << GLOBAL_COUNTER << std::endl;
        file.flush();

        if (GLOBAL_COUNTER == TOTAL_OPERATIONS) {
            running = false;
        }
    }

    void waitEndOfProcessing()
    {
        while (running) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
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
        initializeEvent(evt);
        sequence1(evt);
        sequence2(evt);
        sequence3(evt);
        sequence4(evt);
        sequence5(evt);
        sequence6(evt);
        sequence7(evt);
        sequence8(evt);
        sequence9(evt);
        sequence10(evt);
        sequence11(evt);
        sequence12(evt);
        sequence13(evt);
        sequence14(evt);
        sequence15(evt);
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
        initializeEvent((*disruptor->ringBuffer())[nextSequence]);
        disruptor->ringBuffer()->publish(nextSequence);  // Publish the event to make it available to the consumers
    }
}

void runDisruptor()
{
    auto eventFactory = []() { return Event(); };
    auto taskScheduler = std::make_shared<Disruptor::ThreadPerTaskScheduler>();
    auto waitStrategy = std::make_shared<Disruptor::BusySpinWaitStrategy>();
    auto disruptor = std::make_shared<Disruptor::disruptor<Event>>(eventFactory, RING_BUFFER_SIZE, taskScheduler, Disruptor::ProducerType::Single, waitStrategy);

    auto sequence1Handler = std::make_shared< Sequence1Handler >();
    auto sequence2Handler = std::make_shared< Sequence2Handler >();
    auto sequence3Handler = std::make_shared< Sequence3Handler >();
    auto sequence4Handler = std::make_shared< Sequence4Handler >();
    auto sequence5Handler = std::make_shared< Sequence5Handler >();
    auto sequence6Handler = std::make_shared< Sequence6Handler >();
    auto sequence7Handler = std::make_shared< Sequence7Handler >();
    auto sequence8Handler = std::make_shared< Sequence8Handler >();
    auto sequence9Handler = std::make_shared< Sequence9Handler >();
    auto sequence10Handler = std::make_shared< Sequence10Handler >();
    auto sequence11Handler = std::make_shared< Sequence11Handler >();
    auto sequence12Handler = std::make_shared< Sequence12Handler >();
    auto sequence13Handler = std::make_shared< Sequence13Handler >();
    auto sequence14Handler = std::make_shared< Sequence14Handler >();
    auto sequence15Handler = std::make_shared< Sequence15Handler >();

    disruptor->handleEventsWith(sequence1Handler)
        ->then(sequence2Handler)
        ->then(sequence3Handler)
        ->then(sequence4Handler)
        ->then(sequence5Handler)
        ->then(sequence6Handler)
        ->then(sequence7Handler)
        ->then(sequence8Handler)
        ->then(sequence9Handler)
        ->then(sequence10Handler)
        ->then(sequence11Handler)
        ->then(sequence12Handler)
        ->then(sequence13Handler)
        ->then(sequence14Handler)
        ->then(sequence15Handler);

    taskScheduler->start();
    disruptor->start();

    auto start = std::chrono::high_resolution_clock::now();

    runDisruptorPublisher(disruptor);

    std::cout << "Publisher ended!" << std::endl;

    sequence15Handler->waitEndOfProcessing();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Total elapsed time: " << duration.count() << " ms" << std::endl;
    std::cout << "GLOBAL_COUNTER: " << GLOBAL_COUNTER << std::endl;

    disruptor->shutdown();
    taskScheduler->stop();
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
        runThreads();
    } else if (mode == "disruptor") {
        std::cout << "Running with Disruptor..." << std::endl;
        runDisruptor();
    } else {
        std::cerr << "Invalid mode: " << mode << std::endl;
        std::cerr << "Usage: " << argv[0] << " [thread|disruptor]" << std::endl;
        return 1;
    }

    return 0;
}
