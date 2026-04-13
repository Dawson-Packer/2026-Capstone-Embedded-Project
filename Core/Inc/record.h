#ifndef RECORD_H_
#define RECORD_H_

#include "utils/time.h"

typedef struct {
    char   timestamp[ISO8601_LEN]; /* YYYY-mm-ddThh:mm:ss\0 */
    float  temperature;
    double latitude;
    double longitude;
    float  voltage;
} record_t;

#endif