#include "Event.h"
#include "DisruptorRunner.h"
#include "ProcessingSteps.h"
#include "DisruptorSequence1Handler.h"
#include "DisruptorSequence2Handler.h"
#include "DisruptorSequence3Handler.h"
#include "DisruptorSequence4Handler.h"
#include "DisruptorSequence5Handler.h"
#include "DisruptorSequence6Handler.h"
#include "DisruptorSequence7Handler.h"
#include "DisruptorSequence8Handler.h"
#include "DisruptorSequence9Handler.h"
#include "DisruptorSequence10Handler.h"
#include "DisruptorSequence11Handler.h"
#include "DisruptorSequence12Handler.h"
#include "DisruptorSequence13Handler.h"
#include "DisruptorSequence14Handler.h"

DisruptorRunner::DisruptorRunner(std::uint32_t ringBufferSize, std::uint32_t totalOperations, std::uint32_t numOfElementsInEvent) :
    ringBufferSize_(ringBufferSize), totalOperations_(totalOperations), numOfElementsInEvent_(numOfElementsInEvent),
    taskScheduler_(std::make_shared<Disruptor::ThreadPerTaskScheduler>()),
    waitStrategy_(std::make_shared<Disruptor::BusySpinWaitStrategy>()),
    disruptor_(std::make_shared<Disruptor::disruptor<Event>>([]() { return Event(); },
    ringBufferSize, taskScheduler_, Disruptor::ProducerType::Single, waitStrategy_))
{
    latencyVectorInUs_ = std::make_shared<std::vector<std::vector<std::int64_t>>>();
    latencyVectorInUs_->resize(1);
    (*latencyVectorInUs_)[0].resize(totalOperations_);
}

std::shared_ptr<std::vector<std::vector<std::int64_t>>> DisruptorRunner::getLatencyVector()
{
    return latencyVectorInUs_;
}

void DisruptorRunner::run()
{
    setup();
    taskScheduler_->start();
    disruptor_->start();

    auto start = std::chrono::high_resolution_clock::now();

    runPublisher();

    disruptorSequence15Handler_->waitEndOfProcessing();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total elapsed time: " << duration.count() << " ms" << std::endl;
    std::cout << "Throughput: " << totalOperations_ / duration.count() * 1000 << " QPS" << std::endl;
    std::cout << "GLOBAL_COUNTER: " << ProcessingSteps::getInstance().getCounter() << std::endl;

    disruptor_->shutdown();
    taskScheduler_->stop();
}

void DisruptorRunner::setup()
{
    disruptorSequence15Handler_ = std::make_shared<DisruptorSequence15Handler>(totalOperations_, latencyVectorInUs_);
    disruptor_->handleEventsWith(std::make_shared<DisruptorSequence1Handler>())
        ->then(std::make_shared<DisruptorSequence2Handler>())
        ->then(std::make_shared<DisruptorSequence3Handler>())
        ->then(std::make_shared<DisruptorSequence4Handler>())
        ->then(std::make_shared<DisruptorSequence5Handler>())
        ->then(std::make_shared<DisruptorSequence6Handler>())
        ->then(std::make_shared<DisruptorSequence7Handler>())
        ->then(std::make_shared<DisruptorSequence8Handler>())
        ->then(std::make_shared<DisruptorSequence9Handler>())
        ->then(std::make_shared<DisruptorSequence10Handler>())
        ->then(std::make_shared<DisruptorSequence11Handler>())
        ->then(std::make_shared<DisruptorSequence12Handler>())
        ->then(std::make_shared<DisruptorSequence13Handler>())
        ->then(std::make_shared<DisruptorSequence14Handler>())
        ->then(disruptorSequence15Handler_);
}

void DisruptorRunner::runPublisher()
{
    for (std::uint32_t j = 0; j < totalOperations_; ++j)
    {
        auto nextSequence = disruptor_->ringBuffer()->next();  // Reserve the next slot in the ring buffer
        initializeEvent((*disruptor_->ringBuffer())[nextSequence], numOfElementsInEvent_);
        disruptor_->ringBuffer()->publish(nextSequence);  // Publish the event to make it available to the consumers
        (*latencyVectorInUs_)[0][nextSequence] = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}
