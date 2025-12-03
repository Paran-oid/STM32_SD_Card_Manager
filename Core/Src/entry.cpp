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

/***********************************************************
 * Flags
 ***********************************************************/
#define TESTING_ 0

/***********************************************************
 * Typedefs
 ***********************************************************/
using stm_sd::string, stm_sd::Status, stm_sd::die;
namespace fs = stm_sd::sd_filesystem;

/***********************************************************
 * Extern Variables
 ***********************************************************/
extern SPI_HandleTypeDef  hspi1;
extern UART_HandleTypeDef huart2;

/***********************************************************
 * setup() and loop() implementations
 ***********************************************************/
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
    string s = stm_sd::UART2_Scan();  // not blocking since it uses interrupts

    if (!s.empty())
    {
        s = stm_sd::unescape(s);
        stm_sd::handleCommand(s);
    }

    // Enter sleep mode
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    HAL_ResumeTick();

    HAL_Delay(50);
}
