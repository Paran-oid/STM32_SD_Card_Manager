extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/string.h>

#include "_iwdg.hpp"
#include "hal_init.hpp"
#include "tests.hpp"

#define DEBUG_

void setup()
{
#ifdef DEBUG_
    __HAL_DBGMCU_FREEZE_IWDG();
#endif

    hal_init_all();

    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
}

void loop()
{
#ifndef DEBUG_
    iwdg.refresh();  // time limit 10 seconds
#endif

    HAL_Delay(IWDG_DELAY);
}
