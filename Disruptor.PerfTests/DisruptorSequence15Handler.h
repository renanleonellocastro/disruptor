#pragma once

#include <Disruptor/IEventHandler.h>

class DisruptorSequence15Handler : public Disruptor::IEventHandler<Event>
{
public:
    DisruptorSequence15Handler(std::uint32_t totalOperations, std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs);
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override;
    void waitEndOfProcessing();

private:
    bool running;
    std::uint32_t totalOperations_;
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs_;
};
