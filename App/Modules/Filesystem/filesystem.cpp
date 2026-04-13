#include "filesystem.hpp"
#include "Drivers/sdcard.hpp"
#include "Utils/logging.hpp"

#include <string.h>

static bool       _initialized = false;
static const char datafile[9]  = "data.csv";

static const char TAG[] = "filesystem";

void Filesystem::Init(void)
{

    if (!SDCard::Mount())
    {
        LOG_ERR(TAG, "SD card failed to mount.");
        _initialized = true;
        return;
    }

    SDCard::card_stats_t stats;
    if (SDCard::GetStats(&stats))
    {
        LOG_INF(TAG, "File system initialized.");
        printf("\r\n");
        printf("__[ Internal Storage ]______________________\r\n");
        printf("\r\n");
        printf("   Total Drive Space: %10lu KiB\r\n", stats.total_space_KiB);
        printf("   Space Available  : %10lu KiB\r\n", stats.available_KiB);
        printf("\r\n");
    }

    if (!SDCard::FileExists(datafile))
    {
        const char *header = "timestamp,temperature,voltage,latitude,longitude";
        SDCard::WriteFile(datafile, header, strlen(header));

        LOG_INF(TAG, "Created data output file, %s, and wrote initial headers.", datafile);
    }
}

bool Filesystem::Write(record_t *record)
{
    char buf[256] = {0};

    snprintf(buf, sizeof(buf), "%s,%4.2f,%3.2f,%9.6f,%9.6f\r\n", record->timestamp,
             record->temperature, record->voltage, record->latitude, record->longitude);
    if (SDCard::AppendFile(datafile, buf, strlen(buf)) != FR_OK)
    {
        LOG_ERR(TAG, "Failed to append record to data file.");
        return false;
    }
    return true;
}

void Filesystem::Deinit(void)
{
    if (!SDCard::Unmount())
    {
        LOG_WRN(TAG, "Error occurred while unmounting SD card.");
    }
    else
    {
        LOG_INF(TAG, "Successfully closed file system.");
    }
}
