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

etl::string<256> uart_input_buf;

#define TESTING_ 0

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    if (sd_reader.mount() != SDR_RES::OK) die("error mounting drive\n");
    if (sd_reader.label().empty()) die("invalid label...\n");  // set label
    // if (sd_reader.unmount() != SDR_RES::OK) die("unmount failed...\n");
}

void loop()
{
    etl::string<100> filepath = sd_reader.label();
    filepath += etl::string<4>(":~$ ");
    uart2.send(filepath);

    uart2.scan(uart_input_buf);

    if (uart_input_buf.empty())
    {
        log("empty...\n");
    }
    else
    {
        log("you enetered: ");
        log(uart_input_buf);
        log("\n");
    }

    iwdg.refresh();  // time limit 10 seconds
    HAL_Delay(IWDG_DELAY);
}
