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
namespace scal
{

class uart;
class spi;

template <typename T>
class ptimer;

class iwdg;

}  // namespace scal

/***************************************************************
 * HAL Generated Variables from "main.c"
 ***************************************************************/
extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

/***************************************************************
 * Exported C++ Converted Peripherals
 ***************************************************************/
extern scal::spi spi1;

extern scal::ptimer<uint32_t> tim2;

extern scal::uart uart2;

namespace stm_sd
{
/***************************************************************
 * Function Declarations
 ***************************************************************/
void HALInit();

}  // namespace stm_sd
