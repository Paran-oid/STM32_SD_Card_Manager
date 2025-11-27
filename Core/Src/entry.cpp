extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
#include <etl/format_spec.h>
#include <etl/string.h>
#include <etl/to_string.h>

#include "command_handler.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "printf.h"
#include "scal/iwdg.hpp"
#include "scal/uart.hpp"
#include "tests.hpp"
#include "utils.hpp"

#define TESTING_ 0

using stm_sd::string, stm_sd::status, stm_sd::hal_init_all, stm_sd::die;

namespace fs = stm_sd::filesystem;

// uart input buf
string s;

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    if (fs::mount() != status::ok) die("couldn't mount SD Card");
    if (fs::label().empty()) die("invalid label...");  // must be manually put to set the label

    printf_("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

void loop()
{
    uart2.scan(s);
    s = stm_sd::unescape(s);
    stm_sd::handle_command(s);
    // TODO: fix backspace not working
}
