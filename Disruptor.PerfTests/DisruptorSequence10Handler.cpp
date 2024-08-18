#include "ProcessingSteps.h"
#include "DisruptorSequence10Handler.h"

void DisruptorSequence10Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess10(evt);
}
