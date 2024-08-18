#include "ProcessingSteps.h"
#include "DisruptorSequence9Handler.h"

void DisruptorSequence9Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess9(evt);
}
