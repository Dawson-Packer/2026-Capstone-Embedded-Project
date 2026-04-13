#pragma once

#include "Utils/record.hpp"

namespace Transmit
{

    void Init();

    void Setup();

    void EnQueue(record_t *record);

    void Deinit();
} // namespace Transmit
