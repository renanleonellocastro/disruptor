#include "ProcessingSteps.h"
#include "DisruptorSequence11Handler.h"

void DisruptorSequence11Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess11(evt);
}
