#include "ProcessingSteps.h"
#include "DisruptorSequence8Handler.h"

void DisruptorSequence8Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess8(evt);
}
