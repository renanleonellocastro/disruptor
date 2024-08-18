#include "ProcessingSteps.h"
#include "DisruptorSequence13Handler.h"

void DisruptorSequence13Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess13(evt);
}
