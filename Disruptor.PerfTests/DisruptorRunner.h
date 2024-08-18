#pragma once

#include <memory>
#include <Disruptor/Disruptor.h>
#include <Disruptor/RingBuffer.h>
#include <Disruptor/ProducerType.h>
#include <Disruptor/BusySpinWaitStrategy.h>
#include <Disruptor/ThreadPerTaskScheduler.h>

#include "Event.h"
#include "DisruptorSequence15Handler.h"

class DisruptorRunner
{
public:
    DisruptorRunner(std::uint32_t ringBufferSize, std::uint32_t totalOperations, std::uint32_t numOfElementsInEvent);
    void run();
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> getLatencyVector();

private:
    void setup();
    void runPublisher();

private:
    std::uint32_t ringBufferSize_;
    std::uint32_t totalOperations_;
    std::uint32_t numOfElementsInEvent_;
    std::shared_ptr<Disruptor::ThreadPerTaskScheduler> taskScheduler_;
    std::shared_ptr<Disruptor::BusySpinWaitStrategy> waitStrategy_;
    std::shared_ptr<Disruptor::disruptor<Event>> disruptor_;
    std::shared_ptr<DisruptorSequence15Handler> disruptorSequence15Handler_;
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs_;
};
