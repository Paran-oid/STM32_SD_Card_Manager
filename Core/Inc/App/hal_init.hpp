#pragma once

/***************************************************************
 * Includes
 ***************************************************************/
extern "C"
{
#include "main.h"
}

/***************************************************************
 * Forward Declarations
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
 * HAL Generated Variables from "main.c"
 ***************************************************************/
extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

/***************************************************************
 * Exported C++ Converted Peripherals
 ***************************************************************/
extern sca::spi spi1;

extern sca::ptimer<uint32_t> tim2;

extern sca::uart uart2;

namespace stm_sd
{
/***************************************************************
 * Function Declarations
 ***************************************************************/
void hal_init_all();

}  // namespace stm_sd
