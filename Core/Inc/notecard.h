#ifndef NOTECARD_H_
#define NOTECARD_H_

#include "note.h"
#include "record.h"

#define NOTEHUB_PROJECTUID "com.gmail.dawsonpacker.dev:spring_2026_capstone_development"
#define NOTECARD_SYNC_MODE "continuous" /* continuous or periodic */

/**
 * @brief Initializes the Blues IoT Notecard api.
 */
void Notecard_Init(void);

void Notecard_Configure(void);

bool Notecard_EnableGPS(bool enable);

bool Notecard_GetGPS(double *lat, double *lon, uint32_t *timestamp);

void Notecard_Send(record_t *record);

#endif