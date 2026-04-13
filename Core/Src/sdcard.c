#include "sdcard.h"
#include "ff.h"
#include "logger.h"
#include <stdio.h>

#include "fatfs.h"

uint8_t SDCard_Mount(void)
{

    HAL_Delay(1000);

    FATFS FatFs;
    // FIL     file_handle;
    FRESULT result;

    DSTATUS status = disk_status(0);
    printf("Status before mount: 0x%02x\r\n", status);

    result = f_mount(&FatFs, USERPath, 1); /* 1 = mount now */
    if (result != FR_OK)
    {
        Logger_Error("f_mount error: %i", result);
        return 1;
    }

    DWORD free_clusters, free_sectors, total_sectors;

    FATFS *getFreeFs;

    result = f_getfree(USERPath, &free_clusters, &getFreeFs);
    if (result != FR_OK)
    {
        Logger_Error("f_getfree error: %i", result);
        return 2;
    }

    total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
    free_sectors  = free_clusters * getFreeFs->csize;

    printf("SD Card stats:\r\n");
    printf("%10lu KiB total drive space.\r\n", total_sectors / 2);
    printf("%10lu KiB available.\r\n", free_sectors / 2);

    return 0;
}
