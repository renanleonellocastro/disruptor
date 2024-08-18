#include "ProcessingSteps.h"
#include "DisruptorSequence1Handler.h"

void DisruptorSequence1Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess1(evt);
}
