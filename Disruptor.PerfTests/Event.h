#pragma once

#include "stdafx.h"

struct Event
{
    std::vector<int> data;
};

void initializeEvent(Event& evt, std::uint32_t size);
