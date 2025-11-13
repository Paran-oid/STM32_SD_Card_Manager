extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include "etl/array.h"
#include "etl/format_spec.h"
#include "etl/string.h"
#include "etl/to_string.h"
#include "hal_init.hpp"
#include "printf.h"
#include "sca/iwdg.hpp"
#include "sca/uart.hpp"
#include "tests.hpp"
#include "utils.hpp"

#define MAX_INPUT_BUF_SIZE 200
etl::string<MAX_INPUT_BUF_SIZE> uart_input_buf;

#define TESTING_ 0

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    if (sd_reader.mount() != SD_RES::OK) die("error mounting drive\r\n");
    if (sd_reader.label().empty()) die("invalid label...\r\n");  // set label
    if (sd_reader.unmount() != SD_RES::OK) die("unmount failed...\r\n");

    printf("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

void loop()
{
    uart2.scan(uart_input_buf);
    HAL_Delay(500);
    // handle_command(uart_input_buf);
}
