#include "notecard.hpp"

#include "Utils/logging.hpp"
#include "i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char TAG[] = "notecard";

static size_t _DebugOutput(const char *msg)
{
#ifdef DEBUG

    printf(msg);

#endif
    return strlen(msg);
}

static bool _I2cReset(uint16_t dev)
{
    HAL_I2C_DeInit(&hi2c1);
    HAL_I2C_Init(&hi2c1);
    return true;
}

static const char *_I2cTransmit(uint16_t dev, uint8_t *buf, uint16_t size)
{
    uint8_t writebuf[256];
    writebuf[0] = (uint8_t)size;
    memcpy(&writebuf[1], buf, size);

    HAL_StatusTypeDef err = HAL_I2C_Master_Transmit(&hi2c1, dev << 1, writebuf, size + 1, 250);

    if (err != HAL_OK) return "i2c transmit error";
    return NULL;
}

static const char *_I2cReceive(uint16_t dev, uint8_t *buf, uint16_t size, uint32_t *available)
{
    const char       *errstr = NULL;
    HAL_StatusTypeDef err;
    /* Attempt transmission */
    uint8_t header[2] = {0x00, (uint8_t)size};
    for (int i = 0; i < 3; i++)
    {
        err = HAL_I2C_Master_Transmit(&hi2c1, dev << 1, header, sizeof(header), 250);
        if (err == HAL_OK)
        {
            errstr = NULL;
            break;
        }

        errstr = "i2c: HAL_I2C_Master_Transmit error";
    }

    /* Only receive if transmission succeeded */
    if (errstr != NULL)
    {
        return errstr;
    }

    int      readlen = size + (sizeof(uint8_t) * 2);
    uint8_t *readbuf = static_cast<uint8_t *>(malloc(readlen));

    if (readbuf == NULL)
    {
        errstr = "i2c: insufficient memory (read)";
    }
    else
    {
        err = HAL_I2C_Master_Receive(&hi2c1, dev << 1, readbuf, readlen, 10);
        if (err != HAL_OK)
        {
            errstr = "i2c: HAL_I2C_Master_Receive error";
        }
        else
        {
            uint8_t availbyte = readbuf[0];
            uint8_t goodbyte  = readbuf[1];
            if (goodbyte != size)
            {
                errstr = "i2c: incorrect amount of data";
            }
            else
            {
                *available = availbyte;
                memcpy(buf, &readbuf[2], size);
            }
        }
    }

    free(readbuf);
    return errstr;
}

void Notecard::Init(void)
{
    NoteSetFn(malloc, free, HAL_Delay, HAL_GetTick);
    NoteSetFnI2C(NOTE_I2C_ADDR_DEFAULT, NOTE_I2C_MAX_DEFAULT, _I2cReset, _I2cTransmit, _I2cReceive);
    NoteSetFnDebugOutput(_DebugOutput);
}

void Notecard::Configure(uint32_t sync_interval_minutes)
{
    J *req = NoteNewRequest("hub.set");
    JAddStringToObject(req, "product", NOTEHUB_PROJECTUID);
    JAddStringToObject(req, "mode", NOTECARD_SYNC_MODE);
    JAddNumberToObject(req, "outbound", sync_interval_minutes);
    JAddNumberToObject(req, "inbound", 0); /* Disable inbound syncs */
    NoteRequest(req);

    req = NoteNewRequest("card.location.mode");
    JAddStringToObject(req, "mode", "off");
    NoteRequest(req);

    LOG_DBG(TAG, "Notecard configured.");
}

bool Notecard::GetGPS(double *lat, double *lon, uint32_t *timestamp, uint32_t timeout)
{
    J *en_req = NoteNewRequest("card.location.mode");
    JAddStringToObject(en_req, "mode", "periodic");
    JAddNumberToObject(en_req, "seconds", 1);
    NoteRequest(en_req);

    HAL_Delay(2000); /* allow GNSS startup */

    uint32_t start = HAL_GetTick();
    J       *rsp   = NULL;
    double   _lat  = 0.0;
    double   _lon  = 0.0;
    uint32_t _timestamp;
    int      best_dop = 999;

    while (HAL_GetTick() - start < timeout)
    {

        J *req = NoteNewRequest("card.location");
        JAddBoolToObject(req, "sync", true);

        rsp = NoteRequestResponse(req);

        if (rsp)
        {
            _lat       = JGetNumber(rsp, "lat");
            _lon       = JGetNumber(rsp, "lon");
            _timestamp = JGetNumber(rsp, "time");
            int dop    = JGetNumber(rsp, "dop");

            const char *status = JGetString(rsp, "status");

            if (dop < best_dop)
            {
                best_dop = dop;
            }

            bool valid_fix = _lat != 0 && _lon != 0 && dop < 10;

            if (valid_fix)
            {
                break;
            }

            NoteDeleteResponse(rsp);
        }

        HAL_Delay(2000);
    }

    *lat       = _lat;
    *lon       = _lon;
    *timestamp = _timestamp;

    J *ds_req = NoteNewRequest("card.location.mode");
    JAddStringToObject(ds_req, "mode", "off");
    NoteRequest(ds_req);

    return (*lat != 0 && *lon != 0);
}

uint32_t Notecard::GetTime(void)
{
    J *req = NoteNewRequest("card.time");
    J *rsp = NoteRequestResponse(req);

    if (!req) return 0;

    uint32_t t = static_cast<uint32_t>(JGetNumber(rsp, "time"));

    NoteDeleteResponse(req);
    return t;
}

bool Notecard::GetTemperature(float *temp_c)
{
    J *req = NoteNewRequest("card.temp");
    J *rsp = NoteRequestResponse(req);

    if (!rsp)
    {
        return false;
    }

    float t = JGetNumber(rsp, "value");
    NoteDeleteResponse(rsp);

    *temp_c = t;
    return true;
}

bool Notecard::GetVoltage(float *volt_v)
{
    J *req = NoteNewRequest("card.voltage");
    J *rsp = NoteRequestResponse(req);

    if (!rsp)
    {
        return false;
    }

    float v = JGetNumber(rsp, "value");
    NoteDeleteResponse(rsp);

    *volt_v = v;
    return true;
}

void Notecard::Send(record_t *record)
{
    J *req = NoteNewRequest("note.add");

    JAddStringToObject(req, "file", "data.qo"); /* Queue request to data.qo (outbound) file */
    J *body = NoteNewBody();

    JAddStringToObject(body, "timestamp", record->timestamp);
    JAddNumberToObject(body, "temperature", record->temperature);
    JAddNumberToObject(body, "latitude", record->latitude);
    JAddNumberToObject(body, "longitude", record->longitude);
    JAddNumberToObject(body, "voltage", record->voltage);

    JAddItemToObject(req, "body", body);

    NoteRequest(req);

    J *sync_req = NoteNewRequest("hub.sync");
    NoteRequest(sync_req);
}