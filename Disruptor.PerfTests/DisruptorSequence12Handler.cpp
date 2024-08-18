#include "ProcessingSteps.h"
#include "DisruptorSequence12Handler.h"

void DisruptorSequence12Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess12(evt);
}
