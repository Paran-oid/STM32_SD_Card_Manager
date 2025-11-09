#include "utils.hpp"

void log(etl::string_view msg)
{
    for (size_t i = 0; i < msg.length(); i++)
    {
        if (msg[i] == '\n')
        {
            HAL_UART_Transmit(&huart2, (const uint8_t*) "\r", 1, 100);
            HAL_UART_Transmit(&huart2, (const uint8_t*) "\n", 1, 100);
            i++;
        }
        else
        {
            HAL_UART_Transmit(&huart2, (const uint8_t*) &msg[i], 1, 100);
        }
    }
}

void die(etl::string_view msg)
{
    log(msg);
    Error_Handler();
}
