#pragma once

extern "C"
{
#include "main.h"
}

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint16_t IWDG_DELAY = 1000;  // in ms

/***************************************************************
 * Forward declarations
 ***************************************************************/
template <typename T>
class PTimer;

class GPIO;
class _IWDG;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern IWDG_HandleTypeDef hiwdg;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

/***************************************************************
 * Exported objects
 ***************************************************************/
extern PTimer<uint32_t> tim2;

extern _IWDG iwdg;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
