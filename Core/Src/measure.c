#include "measure.h"
#include "utils/time.h"

#include "adc.h"
#include "logger.h"
#include "notecard.h"

#define GPS_TIMEOUT_MS 120000

#if 0
static float _VBatt_Get(void) {}
#endif

double *last_latitude  = NULL;
double *last_longitude = NULL;

void Measure_TestSend(void) {
    record_t data = {
        .temperature = 15.6,
        .voltage     = 4.97,
    };

    bool success = Measure_GetLocationAndTime(&data);
    if (!success) {
        Logger_Error("Failed to get GPS fix.");
        return;
    }

    Notecard_Send(&data);
}

bool Measure_GetLocationAndTime(record_t *record) {
    double   lat, lon;
    uint32_t gps_time;

    if (!Notecard_EnableGPS(true)) {
        return false;
    }

    uint32_t start = HAL_GetTick();
    while (!Notecard_GetGPS(&lat, &lon, &gps_time)) {
        if (HAL_GetTick() - start > GPS_TIMEOUT_MS) {
            /* timed out */
            return false;
        }
        HAL_Delay(2000);
    }

    record->latitude  = lat;
    record->longitude = lon;

    UTCTime time;
    epochToUtc(gps_time, &time);
    utcToIso8601(&time, record->timestamp);

    if (!Notecard_EnableGPS(false)) {
        return false;
    }
    return true;
}