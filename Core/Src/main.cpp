extern "C"
{
#include "main.h"

#include "fatfs.h"
}

FATFS fs;
DWORD free_clusters, free_sectors, total_sectors;

void setup()
{
    // test sd card

    // check data about microsd
    // put a file in the microsd
    // read it's contents

    HAL_GPIO_WritePin(MSD_CS_GPIO_Port, MSD_CS_Pin, GPIO_PIN_RESET);
    FRESULT fres = f_mount(&fs, "", 1);
    HAL_GPIO_WritePin(MSD_CS_GPIO_Port, MSD_CS_Pin, GPIO_PIN_SET);

    if (fres != FR_OK)
    {
        while (1);
    }

    HAL_GPIO_WritePin(MSD_CS_GPIO_Port, MSD_CS_Pin, GPIO_PIN_RESET);
    FATFS* fsptr = &fs;
    fres         = f_getfree("", &free_clusters, &fsptr);
    HAL_GPIO_WritePin(MSD_CS_GPIO_Port, MSD_CS_Pin, GPIO_PIN_RESET);

    if (fres != FR_OK)
    {
        while (1);
    }

    total_sectors = (fsptr->n_fatent - 2) * fsptr->csize;
    free_sectors  = free_clusters * fsptr->csize;
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
