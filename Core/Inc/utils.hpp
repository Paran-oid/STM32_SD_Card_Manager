#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/string.h>

extern UART_HandleTypeDef huart2;

void log(etl::string_view msg);
void die(etl::string_view msg);
