#pragma once

#include "Utils/record.hpp"
#include "note.h"

#define NOTEHUB_PROJECTUID "com.gmail.dawsonpacker.dev:spring_2026_capstone_development"
#define NOTECARD_SYNC_MODE "periodic" /* continuous or periodic */

namespace Notecard
{
    void Init(void);

    void Configure(uint32_t sync_interval_minutes);

    bool GetGPS(double *lat, double *lon, uint32_t *timestamp, uint32_t timeout = 120000);

    uint32_t GetTime(void);

    bool GetTemperature(float *temp_c);

    bool GetVoltage(float *volt_v);

    void Send(record_t *record, bool sync_now = true);
} // namespace Notecard
