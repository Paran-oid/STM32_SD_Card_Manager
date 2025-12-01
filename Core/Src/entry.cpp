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
#include "printf.h"
#include "sd_filesystem.hpp"
#include "tests.hpp"
#include "uart_io.hpp"
#include "utils.hpp"

#define TESTING_ 0
using stm_sd::string, stm_sd::Status, stm_sd::die;
namespace fs = stm_sd::sd_filesystem;

extern SPI_HandleTypeDef  hspi1;
extern TIM_HandleTypeDef  htim2;
extern UART_HandleTypeDef huart2;

void setup()
{
#if TESTING_
    runTests();  // to configure tests modify runTests in tests.cpp inside Tests folder
#endif
    if (fs::mount() != Status::OK) die("couldn't mount SD Card");
    if (fs::label().empty()) die("invalid label...");  // must be manually put to set the label
    printf_("=======STM32 MICRO SD CARD READER READY!=======\r\n");
}

void loop()
{
    string s;
    s = stm_sd::UART_Scan(&huart2);

    // s = stm_sd::unescape(s);
    // stm_sd::handleCommand(s);
}
