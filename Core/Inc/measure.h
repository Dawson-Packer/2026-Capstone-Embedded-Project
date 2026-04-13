#ifndef MEASURE_H_
#define MEASURE_H_

#include <stdbool.h>
#include "record.h"

void Measure_TestSend(void);

bool Measure_GetLocationAndTime(record_t *record);

#endif