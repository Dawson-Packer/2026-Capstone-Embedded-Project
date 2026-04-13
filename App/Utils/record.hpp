#pragma once

#include "time.hpp"

typedef struct
{
    char   timestamp[ISO8601_LEN]; /* YYYY-mm-ddThh:mm:ss\0 */
    float  temperature;
    double latitude;
    double longitude;
    float  voltage;
} record_t;