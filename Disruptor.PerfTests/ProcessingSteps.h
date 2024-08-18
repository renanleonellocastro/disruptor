#ifndef PROCESSINGSTEPS_H
#define PROCESSINGSTEPS_H

#include <mutex>
#include <vector>
#include <algorithm>

#include "Event.h"

class ProcessingSteps
{
public:
    static ProcessingSteps& getInstance();

    void runProcess1(Event& evt);
    void runProcess2(Event& evt);
    void runProcess3(Event& evt);
    void runProcess4(Event& evt);
    void runProcess5(Event& evt);
    void runProcess6(Event& evt);
    void runProcess7WithContention();
    void runProcess7WithoutContention();
    void runProcess8(Event& evt);
    void runProcess9(Event& evt);
    void runProcess10(Event& evt);
    void runProcess11(Event& evt);
    void runProcess12(Event& evt);
    void runProcess13(Event& evt);
    void runProcess14(Event& evt);
    void runProcess15(Event& evt);
    int getCounter();

private:
    ProcessingSteps() = default;
    ~ProcessingSteps() = default;
    ProcessingSteps(const ProcessingSteps&) = delete;
    ProcessingSteps& operator=(const ProcessingSteps&) = delete;

private:
    static int counter_;
    static std::mutex mutex_;
    static std::ofstream file_;
};

#endif // PROCESSINGSTEPS_H