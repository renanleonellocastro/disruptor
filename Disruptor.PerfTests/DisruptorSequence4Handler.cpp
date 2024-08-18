#include "ProcessingSteps.h"
#include "DisruptorSequence4Handler.h"

void DisruptorSequence4Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess4(evt);
}
