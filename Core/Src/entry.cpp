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

etl::string<MAX_INPUT> uart_input_buf;

#define TESTING_ 0

void setup()
{
    // Initialization of peripherals and API
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
    uart2.scan(uart_input_buf);
    // command [options] [arguments]
    // check if command:
    //      if command not found return none
    // execute command with args and return it's result to uart screen and with SDR_RES
}
