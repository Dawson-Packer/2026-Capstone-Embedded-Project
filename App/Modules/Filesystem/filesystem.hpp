#pragma once
#include "Utils/record.hpp"
#include <stdint.h>

namespace Filesystem
{
    void Init();

    bool Write(record_t *record);

    void Deinit();
}; // namespace Filesystem
