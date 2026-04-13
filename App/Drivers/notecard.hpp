#pragma once

#include "Utils/record.hpp"
#include "note.h"

#define NOTEHUB_PROJECTUID "com.gmail.dawsonpacker.dev:spring_2026_capstone_development"
#define NOTECARD_SYNC_MODE "continuous" /* continuous or periodic */

namespace Notecard
{
    void Init(void);

    void Configure(void);

    void EnableGPS(void);

    void GetGPS(double *lat, double *lon, uint32_t *timestamp);

    void Send(record_t *record);
} // namespace Notecard
