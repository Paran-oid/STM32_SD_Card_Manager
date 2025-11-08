#include "uart.hpp"

MP_RES UART::send(etl::span<const uint8_t> tx)
{
    return HAL_UART_Transmit(&m_huart, tx.data(), tx.size(), 100) == HAL_OK ? MP_RES::OK
                                                                            : MP_RES::ERR;
}

MP_RES UART::send(const uint8_t tx)
{
    return HAL_UART_Transmit(&m_huart, &tx, 1, 100) == HAL_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES UART::receive(etl::span<uint8_t>& rx, bool wait_forever)
{
    return HAL_UART_Receive(&m_huart, rx.data(), rx.size(), wait_forever ? HAL_MAX_DELAY : 100) ==
                   HAL_OK
               ? MP_RES::OK
               : MP_RES::ERR;
}

MP_RES UART::receive(uint8_t& rx, bool wait_forever)
{
    return HAL_UART_Receive(&m_huart, &rx, 1, wait_forever ? HAL_MAX_DELAY : 100) == HAL_OK
               ? MP_RES::OK
               : MP_RES::ERR;
}
