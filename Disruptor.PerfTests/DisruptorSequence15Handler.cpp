#include "ProcessingSteps.h"
#include "DisruptorSequence15Handler.h"

DisruptorSequence15Handler::DisruptorSequence15Handler(std::uint32_t totalOperations,
    std::shared_ptr<std::vector<std::vector<std::int64_t>>> latencyVectorInUs) : running(true),
    totalOperations_(totalOperations), latencyVectorInUs_(latencyVectorInUs)
{
}

void DisruptorSequence15Handler::onEvent(Event& evt, std::int64_t sequence, bool endOfBatch)
{
    ProcessingSteps::getInstance().runProcess15(evt);

    if (ProcessingSteps::getInstance().getCounter() == totalOperations_) {
        running = false;
    }

    (*latencyVectorInUs_)[0][sequence] = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count() - (*latencyVectorInUs_)[0][sequence];
}

void DisruptorSequence15Handler::waitEndOfProcessing()
{
    while (running) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
