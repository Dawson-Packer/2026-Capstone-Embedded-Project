#include "sdcard.hpp"
#include "Utils/logging.hpp"
#include "fatfs.h"
#include "ff.h"

static const char TAG[] = "sdcard";

namespace SDCard
{

    static FATFS FatFs;

    bool Mount()
    {

        HAL_Delay(1000);
        FRESULT result;

        result = f_mount(&FatFs, USERPath, 1);

        if (result != FR_OK)
        {
            LOG_DBG(TAG, "f_mount error while mounting: %i", result);
            return false;
        }

        return true;
    }

    bool Unmount()
    {

        FRESULT result;

        result = f_mount(NULL, USERPath, 0);

        if (result != FR_OK)
        {
            LOG_DBG(TAG, "f_mount error while unmounting: %i", result);
            return false;
        }

        return true;
    }

    bool GetStats(card_stats_t *data)
    {
        DWORD free_clusters;

        FATFS *getFreeFs;

        FRESULT result = f_getfree(USERPath, &free_clusters, &getFreeFs);

        if (result != FR_OK)
        {
            LOG_DBG(TAG, "f_getfree error: %i", result);
            return false;
        }

        data->total_space_KiB = ((getFreeFs->n_fatent - 2) * getFreeFs->csize) / 2;
        data->available_KiB   = (free_clusters * getFreeFs->csize) / 2;

        return true;
    }

    bool FileExists(const char *filename)
    {
        FILINFO info;
        FRESULT res = f_stat(filename, &info);
        return (res == FR_OK);
    }

    FRESULT WriteFile(const char *filename, const void *data, size_t len)
    {
        FIL    file;
        size_t written;

        FRESULT res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (res != FR_OK)
        {
            LOG_DBG(TAG, "f_open error when creating file: %i", res);
            return res;
        }

        res = f_write(&file, data, len, &written);
        if (res == FR_OK && written != len)
        {
            LOG_DBG(TAG, "Failed to write all data to file %s", filename);
            res = FR_DISK_ERR;
        }

        f_sync(&file);
        f_close(&file);
        return res;
    }

    FRESULT AppendFile(const char *filename, const void *data, size_t len)
    {
        FIL    file;
        size_t written;

        FRESULT res = f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE | FA_OPEN_ALWAYS);
        if (res != FR_OK)
        {
            LOG_DBG(TAG, "f_open error when appending to file: %i", res);
            return res;
        }

        res = f_write(&file, data, len, &written);
        if (res == FR_OK && written != len)
        {
            LOG_DBG(TAG, "Failed to write all data to file %s", filename);
            res = FR_DISK_ERR;
        }

        f_sync(&file);
        f_close(&file);
        return res;
    }

} // namespace SDCard
