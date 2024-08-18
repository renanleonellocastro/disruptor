#include "ProcessingSteps.h"
#include "DisruptorSequence5Handler.h"

void DisruptorSequence5Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess5(evt);
}
