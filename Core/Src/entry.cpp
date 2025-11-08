extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
#include <etl/format_spec.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include "_iwdg.hpp"
#include "hal_init.hpp"
#include "tests.hpp"
#include "uart.hpp"

#define TESTING_ 0

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif
    sd_reader.mount();

    sd_reader.unmount();
}

void loop()
{
    iwdg.refresh();  // time limit 10 seconds
    HAL_Delay(IWDG_DELAY);
}
