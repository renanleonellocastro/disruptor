#include "ProcessingSteps.h"
#include "DisruptorSequence2Handler.h"

void DisruptorSequence2Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess2(evt);
}
