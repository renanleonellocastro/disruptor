#include "ProcessingSteps.h"
#include "DisruptorSequence3Handler.h"

void DisruptorSequence3Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess3(evt);
}
