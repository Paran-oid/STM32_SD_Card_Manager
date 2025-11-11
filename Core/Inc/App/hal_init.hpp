#pragma once

extern "C"
{
#include "main.h"
}

#include "sd.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/

/***************************************************************
 * Forward declarations
 ***************************************************************/
namespace sca
{

class uart;
class spi;

template <typename T>
class ptimer;

class iwdg;

}  // namespace sca

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

/***************************************************************
 * Exported C++ converted peripherals
 ***************************************************************/

extern sca::spi spi1;

extern sca::ptimer<uint32_t> tim2;

extern sca::uart uart2;

/***************************************************************
 * Exported objects
 ***************************************************************/

extern MicroSDHandler sd_reader;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
