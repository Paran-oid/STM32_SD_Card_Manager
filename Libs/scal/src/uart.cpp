#include "scal/uart.hpp"
namespace scal
{

#ifdef SCA_UART

SCA_RES uart::send(etl::string_view str)
{
    return HAL_UART_Transmit(&m_huart, (const uint8_t*) str.data(), (size_t) str.size(), 100) ==
                   HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

SCA_RES uart::send(etl::span<const uint8_t> tx)
{
    return HAL_UART_Transmit(&m_huart, tx.data(), tx.size(), 100) == HAL_OK ? SCA_RES::OK
                                                                            : SCA_RES::ERR;
}

SCA_RES uart::send(const uint8_t tx)
{
    return HAL_UART_Transmit(&m_huart, &tx, 1, 100) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

SCA_RES uart::receive(etl::span<uint8_t>& rx, bool wait_forever)
{
    return HAL_UART_Receive(&m_huart, rx.data(), rx.size(), wait_forever ? HAL_MAX_DELAY : 100) ==
                   HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

SCA_RES uart::receive(uint8_t& rx, bool wait_forever)
{
    return HAL_UART_Receive(&m_huart, &rx, 1, wait_forever ? HAL_MAX_DELAY : 100) == HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

#endif

}  // namespace scal
