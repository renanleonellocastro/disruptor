#include "ProcessingSteps.h"
#include "DisruptorSequence7Handler.h"

void DisruptorSequence7Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess7WithoutContention();
}
