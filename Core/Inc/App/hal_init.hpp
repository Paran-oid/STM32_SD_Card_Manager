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

class UART;
class SPI;

template <typename T>
class PTimer;

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
extern scal::SPI spi1;

extern scal::PTimer<uint32_t> tim2;

extern scal::UART uart2;

namespace stm_sd
{
/***************************************************************
 * Function Declarations
 ***************************************************************/
void HALInit();

}  // namespace stm_sd
