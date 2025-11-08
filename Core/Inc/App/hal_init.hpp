#pragma once

extern "C"
{
#include "main.h"
}

#include "sd.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint16_t IWDG_DELAY = 1000;  // in ms

/***************************************************************
 * Forward declarations
 ***************************************************************/
class UART;
class SPI;

template <typename T>
class PTimer;

class _IWDG;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern IWDG_HandleTypeDef hiwdg;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

/***************************************************************
 * Exported C++ converted peripherals
 ***************************************************************/

extern _IWDG iwdg;

extern SPI spi1;

extern PTimer<uint32_t> tim2;

extern UART uart2;

/***************************************************************
 * Exported objects
 ***************************************************************/

extern MicroSDReader sd_reader;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
