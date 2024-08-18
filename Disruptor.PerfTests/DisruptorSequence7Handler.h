#pragma once

#include <Disruptor/IEventHandler.h>

class DisruptorSequence7Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override;
};
