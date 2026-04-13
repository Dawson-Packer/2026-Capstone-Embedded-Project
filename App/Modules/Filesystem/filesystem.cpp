#include "filesystem.hpp"
#include "Drivers/sdcard.hpp"
#include "Utils/logging.hpp"

#include <string.h>

static const char TAG[] = "filesystem";

namespace fs
{

    Filesystem::Filesystem()
    {

        if (SDCard::Mount() != 0)
        {
            LOG_ERR(TAG, "SD card failed to mount.");
            _initialized = 0;
            return;
        }

        SDCard::card_stats_t stats;
        if (SDCard::GetStats(&stats) == 0)
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

    Filesystem::~Filesystem()
    {
        if (SDCard::Unmount() != 0)
        {
            LOG_WRN(TAG, "Error occurred while unmounting SD card.");
        }
        else
        {
            LOG_INF(TAG, "Successfully closed file system.");
        }
    }

} // namespace fs
