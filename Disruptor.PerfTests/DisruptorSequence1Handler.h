#pragma once

#include "Event.h"
#include <Disruptor/IEventHandler.h>

class DisruptorSequence1Handler : public Disruptor::IEventHandler<Event>
{
public:
    void onEvent(Event& evt, std::int64_t sequence, bool endOfBatch) override;
};
