extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
#include <etl/format_spec.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include "hal_init.hpp"
#include "sca/iwdg.hpp"
#include "sca/uart.hpp"
#include "tests.hpp"

#define TESTING_ 0
void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    // TODO:
    /*
     * Create a project that lets you transfer files to stm32 from pc and vice versa
     * Verify that the data is actually stored in micro sd card
     */

    sd_reader.mount();

    sd_reader.unmount();
}

void loop()
{
    log("Hello!\n");

    iwdg.refresh();  // time limit 10 seconds
    HAL_Delay(IWDG_DELAY);
}
