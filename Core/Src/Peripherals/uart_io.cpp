#include "uart_io.hpp"

#include "defs.hpp"
#include "status.hpp"

/***********************************************************
 * Extern Variables
 ***********************************************************/
extern UART_HandleTypeDef huart2;

/***********************************************************
 * Static Variables
 ***********************************************************/
static volatile uint8_t bufRX[stm_sd::SSIZE];
static volatile uint8_t bufSize                 = 0;
static volatile bool    isBufReady              = false;
static volatile bool    hasStartedProcessingBuf = false;

/***********************************************************
 * Interrupt Callbacks
 ***********************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART2)
    {
        uint8_t c = huart->Instance->RDR;

        switch (c)
        {
            case '\r':
            case '\n':
                HAL_UART_Transmit(huart, (const uint8_t*) "\r\n", 2, stm_sd::DEFAULT_TIMEOUT);
                bufRX[bufSize] = '\0';
                isBufReady     = true;
                break;

            case stm_sd::BACKSPACE:
            case '\b':
                if (bufSize > 0)
                {
                    HAL_UART_Transmit(huart, (const uint8_t*) "\b \b", 3, stm_sd::DEFAULT_TIMEOUT);
                    bufSize -= 1;
                    bufRX[bufSize] = '\0';
                }
                break;
            default:
                if (bufSize < stm_sd::SSIZE - 1)
                {
                    bufRX[bufSize] = c;
                    bufSize += 1;
                    HAL_UART_Transmit(huart, &c, 1, stm_sd::DEFAULT_TIMEOUT);
                }
                break;
        }

        HAL_UART_Receive_IT(huart, &c, 1);
    }
}

namespace stm_sd
{

string UART2_Scan()
{
    if (!hasStartedProcessingBuf)
    {
        uint8_t dummy;
        HAL_UART_Receive_IT(&huart2, &dummy, 1);
    }
    if (isBufReady)
    {
        /*
            I would have rather just returned a reinterprted_cast<volatile *>(bufRX) instead of
           doing all of this but apparently this isn't implemented by the ETL library
        */

        // Create a temporary non-volatile buffer to copy the contents
        char tempBuf[stm_sd::SSIZE];

        // Copy the contents of the volatile buffer to a non-volatile buffer
        for (size_t i = 0; i < stm_sd::SSIZE; ++i)
        {
            tempBuf[i] = static_cast<char>(bufRX[i]);  // Copy each byte to tempBuf
            if (bufRX[i] == '\0')  // Stop if null-terminated (important for string-like behavior)
                break;
        }

        isBufReady              = false;
        hasStartedProcessingBuf = false;
        etl::mem_set(bufRX, bufSize, 0);  // TODO: fix this

        // Return a string created from the non-volatile buffer
        return string(tempBuf);
    }
    return "";
}

}  // namespace stm_sd
