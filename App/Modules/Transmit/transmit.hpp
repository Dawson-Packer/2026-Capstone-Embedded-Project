#pragma once

#include "Utils/record.hpp"

namespace Transmit
{

    void Init();

    void Setup();

    void EnQueue(record_t *record);

    void Notify(power_event_t *event);

    void Deinit();
} // namespace Transmit
