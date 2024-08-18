project(Disruptor.PerfTests)
cmake_minimum_required(VERSION 2.6)

find_package(Boost COMPONENTS system thread date_time)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories("..")


set(DisruptorPerfTests_sources
    main.cpp
    DisruptorRunner.cpp
    DisruptorRunner.h
    ThreadPoolRunner.cpp
    ThreadPoolRunner.h
    Event.h
    Event.cpp
    ProcessingSteps.cpp
    ProcessingSteps.h
    DisruptorSequence1Handler.cpp
    DisruptorSequence1Handler.h
    DisruptorSequence2Handler.cpp
    DisruptorSequence2Handler.h
    DisruptorSequence3Handler.cpp
    DisruptorSequence3Handler.h
    DisruptorSequence4Handler.cpp
    DisruptorSequence4Handler.h
    DisruptorSequence5Handler.cpp
    DisruptorSequence5Handler.h
    DisruptorSequence6Handler.cpp
    DisruptorSequence6Handler.h
    DisruptorSequence7Handler.cpp
    DisruptorSequence7Handler.h
    DisruptorSequence8Handler.cpp
    DisruptorSequence8Handler.h
    DisruptorSequence9Handler.cpp
    DisruptorSequence9Handler.h
    DisruptorSequence10Handler.cpp
    DisruptorSequence10Handler.h
    DisruptorSequence11Handler.cpp
    DisruptorSequence11Handler.h
    DisruptorSequence12Handler.cpp
    DisruptorSequence12Handler.h
    DisruptorSequence13Handler.cpp
    DisruptorSequence13Handler.h
    DisruptorSequence14Handler.cpp
    DisruptorSequence14Handler.h
    DisruptorSequence15Handler.cpp
    DisruptorSequence15Handler.h
)

add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(Disruptor.PerfTests ${DisruptorPerfTests_sources})
target_link_libraries(Disruptor.PerfTests DisruptorStatic Disruptor.TestTools ${Boost_LIBRARIES} pthread Boost::thread)

add_custom_target(performance_test ${CMAKE_CURRENT_BINARY_DIR}/Disruptor.PerfTests)
