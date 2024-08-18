#include "ProcessingSteps.h"
#include "DisruptorSequence6Handler.h"

void DisruptorSequence6Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess6(evt);
}
