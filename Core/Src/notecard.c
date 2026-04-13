#include "notecard.h"

#include "i2c.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t _Notecard_DebugOutput(const char *msg) {
#ifdef DEBUG
    printf(msg);

#endif
    return strlen(msg);
}

static bool _Notecard_I2cReset(uint16_t dev_address) {
    HAL_I2C_DeInit(&hi2c1);
    HAL_I2C_Init(&hi2c1);
    return true;
}

static const char *_Notecard_I2cTransmit(uint16_t addr, uint8_t *buf, uint16_t size) {
    uint8_t writebuf[256];
    writebuf[0] = (uint8_t)size;
    memcpy(&writebuf[1], buf, size);

    HAL_StatusTypeDef err = HAL_I2C_Master_Transmit(&hi2c1, addr << 1, writebuf, size + 1, 250);

    if (err != HAL_OK)
        return "i2c transmit error";
    return NULL;
}

static const char *_Notecard_I2cReceive(uint16_t addr, uint8_t *buf, uint16_t size,
                                        uint32_t *available) {

    const char       *errstr = NULL;
    HAL_StatusTypeDef err;
    /* Attempt transmission */
    uint8_t header[2] = {0x00, (uint8_t)size};
    for (int i = 0; i < 3; i++) {
        // uart_printf("i2c: Sending header to device 0x%02X, attempt %d...\r\n", addr, i);
        err = HAL_I2C_Master_Transmit(&hi2c1, addr << 1, header, sizeof(header), 250);
        if (err == HAL_OK) {
            errstr = NULL;
            break;
        }

        errstr = "i2c: HAL_I2C_Master_Transmit error";
    }

    /* Only receive if transmission succeeded */
    if (errstr != NULL) {
        return errstr;
    }

    int      readlen = size + (sizeof(uint8_t) * 2);
    uint8_t *readbuf = malloc(readlen);

    if (readbuf == NULL) {
        errstr = "i2c: insufficient memory (read)";
    } else {
        err = HAL_I2C_Master_Receive(&hi2c1, addr << 1, readbuf, readlen, 10);
        if (err != HAL_OK) {
            errstr = "i2c: HAL_I2C_Master_Receive error";
        } else {
            uint8_t availbyte = readbuf[0];
            uint8_t goodbyte  = readbuf[1];
            if (goodbyte != size) {
                errstr = "i2c: incorrect amount of data";
            } else {
                *available = availbyte;
                memcpy(buf, &readbuf[2], size);
            }
        }
    }

    free(readbuf);
    return errstr;
}

void Notecard_Init(void) {
    NoteSetFn(malloc, free, HAL_Delay, HAL_GetTick);
    NoteSetFnI2C(NOTE_I2C_ADDR_DEFAULT, NOTE_I2C_MAX_DEFAULT, _Notecard_I2cReset,
                 _Notecard_I2cTransmit, _Notecard_I2cReceive);
    NoteSetFnDebugOutput(_Notecard_DebugOutput);
}

void Notecard_Configure(void) {

    J *req = NoteNewRequest("hub.set");
    JAddStringToObject(req, "product", NOTEHUB_PROJECTUID);
    JAddStringToObject(req, "mode", NOTECARD_SYNC_MODE);
    // JAddNumberToObject(req, "outbound", 180);
    // JAddNumberToObject(req, "inbound", 240);
    NoteRequest(req);

    Logger_Info("Notecard configured.");
}

bool Notecard_EnableGPS(bool enable) {
    J *req = NoteNewRequest("card.location.mode");
    JAddStringToObject(req, "mode", enable ? "periodic" : "off");
    if (enable)
        JAddNumberToObject(req, "seconds", 1);
    if (!NoteRequest(req))
        return false;
    if (enable)
        Logger_Info("Enabled GPS.");
    else
        Logger_Info("Disabled GPS.");
    return true;
}

bool Notecard_GetGPS(double *lat, double *lon, uint32_t *timestamp) {
    J *req = NoteNewRequest("card.location");
    J *rsp = NoteRequestResponse(req);

    if (rsp == NULL)
        return false;
    char *status = JGetString(rsp, "status");
    if (strstr(status, "GPS updated") == NULL) {
        NoteDeleteResponse(rsp);
        return false;
    }

    *lat       = JGetNumber(rsp, "lat");
    *lon       = JGetNumber(rsp, "lon");
    *timestamp = JGetNumber(rsp, "time");

    NoteDeleteResponse(rsp);
    return true;
}

void Notecard_Send(record_t *record) {
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