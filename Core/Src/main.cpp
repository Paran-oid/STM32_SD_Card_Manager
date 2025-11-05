#include "ff.h"
extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <string.h>

extern UART_HandleTypeDef huart2;

FATFS fs;
FIL   file;
DWORD free_clusters;

uint32_t total_sectors, freespace;

char buffer[100];

void setup()
{
    // test sd card

    // check data about microsd
    // put a file in the microsd
    // read it's contents (correctly)

    FRESULT fres = f_mount(&fs, "", 1);

    if (fres != FR_OK)
        while (1);

    FATFS* fsptr = &fs;
    fres         = f_getfree("", &free_clusters, &fsptr);

    if (fres != FR_OK)

        while (1);

    total_sectors = (uint32_t) ((fsptr->n_fatent - 2) * fsptr->csize * 0.5);
    freespace     = (uint32_t) (free_clusters * fsptr->csize * 0.5);

    HAL_Delay(500);

    fres = f_open(&file, "write.txt", FA_CREATE_NEW | FA_WRITE | FA_READ);
    if (fres != FR_OK)

        while (1);

    f_puts("Hello from Aziz", &file);

    fres = f_lseek(&file, 0);

    if (fres != FR_OK)
        while (1);

    // Reads line by line until the end
    while (f_gets(buffer, sizeof(buffer), &file))
    {
        HAL_UART_Transmit(&huart2, (const uint8_t*) buffer, sizeof(buffer), 100);
        memset(buffer, 0, sizeof(buffer));
        // Can use the buffer for something useful
    }

    memset(buffer, 0, sizeof(buffer));
    f_close(&file);
}

void loop()
{
    HAL_Delay(500);
}

extern "C"
{
    void setup_map(void)
    {
        setup();
    }

    void loop_map(void)
    {
        loop();
    }
}
