#include "measure.hpp"
#include "Drivers/notecard.hpp"
#include "Utils/logging.hpp"
#include "Utils/time.hpp"

static const char TAG[] = "measure";

record_t Measure::Take(void)
{
    record_t           record;
    uint32_t           timestamp;
    DateUtils::UTCTime utc_time;

    LOG_DBG(TAG, "Attempting to fetch GPS and time...");
    if (!Notecard::GetGPS(&record.latitude, &record.longitude, &timestamp,
                          10000)) // TODO: 10 seconds timeout
    {
        LOG_WRN(TAG, "GPS fix timed out.");
        record.latitude  = 0.0;
        record.longitude = 0.0;
        timestamp        = Notecard::GetTime();
    }

    DateUtils::epochToUtc(timestamp, &utc_time);
    DateUtils::utcToIso8601(&utc_time, record.timestamp);

    LOG_DBG(TAG, "Attempting to fetch temperature...");
    if (!Notecard::GetTemperature(&record.temperature))
    {
        LOG_WRN(TAG, "Failed to fetch temperature from Notecard.");
        record.temperature = 99.9;
    }

    if (!Notecard::GetVoltage(&record.voltage))
    {
        LOG_WRN(TAG, "Failed to fetch voltage from Notecard.");
        record.voltage = 99.0;
    }

    LOG_INF(TAG, "Finished taking measurement.");
    printf("\r\n");
    printf("__[ Data Collected ]______________________\r\n");
    printf("\r\n");
    printf("   Time       : %s\r\n", record.timestamp);
    printf("   Temperature: %4.2lf C\r\n", record.temperature);
    printf("   Voltage    : %3.2lfV\r\n", record.voltage);
    printf("   Latitude   : %9.6lf°\r\n", record.latitude);
    printf("   Longitude  : %9.6lf°\r\n", record.longitude);
    printf("\r\n");

    return record;
}