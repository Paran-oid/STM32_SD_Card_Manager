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

using stm_sd::string;

namespace fs = stm_sd::filesystem;

// uart input buf
string s;

void setup()
{
    hal_init_all();

#if TESTING_
    run_tests();  // to configure tests modify run_tests in tests.cpp inside Tests folder
#endif

    fs::init(hspi1);

    if (fs::mount() != stm_sd::StatusCode::OK) stm_sd::die("couldn't mount SD Card");
    if (fs::label().empty()) stm_sd::die("invalid label...");  // set label

    printf("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

void loop()
{
    // TODO: don't let user enter unallowd characters for filenames
    // TODO: make sure all variables are initialized and not just declared (var x NO --> var x = 0
    // ;(or {}) YES)
    uart2.scan(s);
    s = stm_sd::unescape(s);
    stm_sd::handle_command(s);
}
