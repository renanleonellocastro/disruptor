#include <numeric>
#include <fstream>

#include "ProcessingSteps.h"

int ProcessingSteps::counter_ = 0;
std::mutex ProcessingSteps::mutex_;
std::ofstream ProcessingSteps::file_("/tmp/log.txt", std::ios_base::app);

ProcessingSteps& ProcessingSteps::getInstance()
{
    static ProcessingSteps instance;
    return instance;
}

void ProcessingSteps::runProcess1(Event& evt)
{
    std::transform(evt.data.begin(), evt.data.end(), evt.data.begin(), [](int val) { return val * 2; });
}

void ProcessingSteps::runProcess2(Event& evt)
{
    std::for_each(evt.data.begin(), evt.data.end(), [](int& val) { val += 100; });
}

void ProcessingSteps::runProcess3(Event& evt)
{
    std::sort(evt.data.begin(), evt.data.end());
}

void ProcessingSteps::runProcess4(Event& evt)
{
    std::partial_sum(evt.data.begin(), evt.data.end(), evt.data.begin());
}

void ProcessingSteps::runProcess5(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess6(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess7WithContention()
{
    std::lock_guard<std::mutex> lock(mutex_);
    counter_++;
    file_ << "GLOBAL COUNTER: " << counter_ << std::endl;
    file_.flush();
}

void ProcessingSteps::runProcess7WithoutContention()
{
    counter_++;
    file_ << "GLOBAL COUNTER: " << counter_ << std::endl;
    file_.flush();
}

void ProcessingSteps::runProcess8(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess9(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess10(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess11(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess12(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess13(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess14(Event& evt)
{
    runProcess1(evt);
}

void ProcessingSteps::runProcess15(Event& evt)
{
    runProcess1(evt);
}

int ProcessingSteps::getCounter()
{
    return counter_;
}
