#include "ProcessingSteps.h"
#include "DisruptorSequence14Handler.h"

void DisruptorSequence14Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess14(evt);
}
