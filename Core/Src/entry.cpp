extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include "command_handler.hpp"
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

#define TESTING_ 0

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    if (sd_reader.mount() != SD_RES::OK) die("error mounting drive\r\n");
    if (sd_reader.label().empty()) die("invalid label...\r\n");  // set label
    // if (sd_reader.unmount() != SD_RES::OK) die("unmount failed...\r\n");

    // TODO: get used to writing docs before even beginning to write code
    printf("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

// uart input buf
estring s;
void    loop()
{
    uart2.scan(s);
    s = unescape(s);
    handle_command(s);
}
