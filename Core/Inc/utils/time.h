#ifndef TIME_H_
#define TIME_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ISO8601_LEN 20

typedef struct {
    uint16_t year;   // 0-9999
    uint8_t  month;  // 1-12
    uint8_t  day;    // 1-31
    uint8_t  hour;   // 1-24
    uint8_t  minute; // 1-59
    uint8_t  second; // 1-59
} UTCTime;

void epochToUtc(uint32_t epoch, UTCTime *t);

void utcToIso8601(UTCTime *utc, char *timestamp);

#endif