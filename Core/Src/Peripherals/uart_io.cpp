#include "uart_io.hpp"

#include "defs.hpp"
#include "status.hpp"

static UART_HandleTypeDef*
            s_huart_p;  // stores address of last used huart peripheral inside UART_Scan
static bool hasPendingChar = false;  // signals if UART_Scan can proceed or not

/***********************************************************
 * Interrupt Callbacks
 ***********************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    // explicitly stated USART2 to han
    if (huart == s_huart_p)
    {
        hasPendingChar = true;
    }
}

namespace stm_sd
{

string UART_Scan(UART_HandleTypeDef* huart, bool display)
{
    s_huart_p = huart;  // assign for interrupt handling

    uint8_t buf[SSIZE] =
        {};  // user can't enter a command bigger than SSIZE - 1 (last spot reserved for \0)
    uint16_t size = 0;

    uint8_t c;
    HAL_UART_Receive_IT(huart, &c, 1);
    for (;;)
    {
        if (!hasPendingChar) continue;

        if (c == '\r' || c == '\n')
        {
            HAL_UART_Transmit(huart, (const uint8_t*) "\r\n", 2, DEFAULT_TIMEOUT);
            buf[size] = '\0';
            break;
        }

        if (c == BACKSPACE || c == '\b')
        {
            if (size > 0)
            {
                HAL_UART_Transmit(huart, (const uint8_t*) "\b \b", 3, DEFAULT_TIMEOUT);
                buf[--size] = '\0';
            }
        }
        else if (size < SSIZE - 1)
        {
            buf[size++] = c;
            if (display)
            {
                HAL_UART_Transmit(huart, &c, 1, DEFAULT_TIMEOUT);
            }
        }

        hasPendingChar = false;
        HAL_UART_Receive_IT(huart, &c, 1);
    }
    // TODO: fix this

    string out;
    out.assign((reinterpret_cast<char*>(buf)));
    return out;
}

}  // namespace stm_sd
