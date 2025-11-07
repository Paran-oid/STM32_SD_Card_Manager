#include "utils.hpp"

void log(etl::string_view msg)
{
    HAL_UART_Transmit(&huart2, (const uint8_t*) msg.data(), msg.length(), 100);
}

void die(etl::string_view msg)
{
    log(msg);
    Error_Handler();
}
