#include "Event.h"

void initializeEvent(Event& evt, std::uint32_t size)
{
    evt.data.resize(size);

    for (std::uint32_t i = 0; i < size; ++i) {
        evt.data[i] = i;
    }
}
