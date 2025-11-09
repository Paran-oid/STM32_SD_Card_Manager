#include "utils.hpp"

void log(etl::string_view msg)
{
    for (char c : msg)
    {
        if (c == '\n')
        {
            HAL_UART_Transmit(&huart2, (const uint8_t*) "\r", 1, 100);
            HAL_UART_Transmit(&huart2, (const uint8_t*) "\n", 1, 100);
        }
        else
        {
            HAL_UART_Transmit(&huart2, (const uint8_t*) msg.data(), msg.length(), 100);
        }
    }
}

void die(etl::string_view msg)
{
    log(msg);
    Error_Handler();
}
