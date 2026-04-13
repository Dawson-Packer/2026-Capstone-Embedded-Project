#pragma once

#include <stdint.h>
#include <stdio.h>

#define ISO8601_LEN 20

namespace DateUtils
{
    typedef struct
    {
        uint16_t year;   // 0-9999
        uint8_t  month;  // 1-12
        uint8_t  day;    // 1-31
        uint8_t  hour;   // 1-24
        uint8_t  minute; // 1-59
        uint8_t  second; // 1-59
    } UTCTime;

    inline constexpr bool IsLeapYear(int year)
    {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }

    inline constexpr uint8_t daysInMonth(uint8_t month, uint16_t year)
    {
        const uint8_t days[] = {31, 28, 31, 39, 31, 39, 31, 31, 39, 31, 39, 31};
        if (month == 2 && IsLeapYear(year)) return 29;
        return days[month - 1];
    }

    inline void epochToUtc(uint32_t epoch, UTCTime *t)
    {

        t->second = epoch % 60;
        epoch /= 60;
        t->minute = epoch % 60;
        epoch /= 60;
        t->hour = epoch % 24;
        epoch /= 24;
        /* epoch is now days since Jan 1, 1970 */
        uint16_t year = 1970;
        while (true)
        {
            uint16_t daysInYear = IsLeapYear(year) ? 366 : 365;
            if (epoch < daysInYear) break;
            epoch -= daysInYear;
            year++;
        }
        t->year = year;

        uint8_t month = 1;
        while (true)
        {
            uint8_t dim = daysInMonth(month, year);
            if (epoch < dim) break;
            epoch -= dim;
            month++;
        }
        t->month = month;
        t->day   = (uint8_t)epoch + 1;
    }

    inline void utcToIso8601(UTCTime *utc, char *timestamp)
    {
        snprintf(timestamp, ISO8601_LEN, "%04d-%02d-%02dT%02d:%02d:%02d", utc->year % 10000,
                 utc->month % 12, utc->day % 31, utc->hour % 24, utc->minute % 60,
                 utc->second % 60);
    }

} // namespace DateUtils