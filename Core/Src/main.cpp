extern "C"
{
#include "main.h"

#include "fatfs.h"
}

#include "_iwdg.hpp"
#include "hal_init.hpp"

char buffer[100];

void setup()
{
    hal_init_all();  // initalizes all peripherals to their respective C++ classes and structs
}

void loop()
{
    iwdg.refresh();  // time limit 10 seconds
    HAL_Delay(IWDG_DELAY);
}
