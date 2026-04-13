#pragma once

#include "stdint.h"

namespace Power
{
    bool WokeFromStandby(void);

    void EnterStandby(uint32_t next_wakeup_seconds);
} // namespace Power
