extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
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

    // run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder

    sd_reader.mount();

    etl::array<FILINFO, PAGE_SIZE> arr;

    sd_reader.list_files("/", 0, arr);
    // TODO: delete all files and recheck with list_files
    // TODO: make sure paging works for reading from a directory

    sd_reader.unmount();
}

void loop()
{
    iwdg.refresh();  // time limit 10 seconds
    HAL_Delay(IWDG_DELAY);
}
