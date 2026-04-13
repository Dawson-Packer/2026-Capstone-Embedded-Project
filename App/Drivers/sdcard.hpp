#pragma once

#include "stdint.h"
#include "ff.h"

namespace SDCard
{

    typedef struct
    {
        uint32_t total_space_KiB;
        uint32_t available_KiB;
    } card_stats_t;

    bool Mount(void);

    bool Unmount(void);

    bool GetStats(card_stats_t *data);

    bool FileExists(const char *filename);

    FRESULT WriteFile(const char *filename, const void *data, size_t len);

    FRESULT AppendFile(const char *filename, const void *data, size_t len);

} // namespace SDCard
