#pragma once
#include "Utils/record.hpp"
#include <stdint.h>

namespace Filesystem
{
    void Init(void);

    bool Write(record_t *record);

    void Deinit(void);
}; // namespace Filesystem
