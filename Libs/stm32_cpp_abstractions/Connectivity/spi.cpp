#include "spi.hpp"

namespace sca
{

SCA_RES spi::send(etl::span<const uint8_t> tx)
{
    return HAL_SPI_Transmit(&m_hspi, tx.data(), tx.size(), 100) == HAL_OK ? SCA_RES::OK
                                                                          : SCA_RES::ERR;
}

SCA_RES spi::send(const uint8_t tx)
{
    return HAL_SPI_Transmit(&m_hspi, &tx, 1, 100) == HAL_OK ? SCA_RES::OK : SCA_RES::ERR;
}

SCA_RES spi::receive(etl::span<uint8_t>& rx, bool wait_forever)
{
    return HAL_SPI_Receive(&m_hspi, rx.data(), rx.size(), wait_forever ? HAL_MAX_DELAY : 100) ==
                   HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

SCA_RES spi::receive(uint8_t& rx, bool wait_forever)
{
    return HAL_SPI_Receive(&m_hspi, &rx, 1, wait_forever ? HAL_MAX_DELAY : 100) == HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

SCA_RES spi::send_and_receive(etl::span<const uint8_t> tx, etl::span<uint8_t>& rx)
{
    if (tx.size() != rx.size()) return SCA_RES::ERR;
    return HAL_SPI_TransmitReceive(&m_hspi, tx.data(), rx.data(), tx.size(), 100) == HAL_OK
               ? SCA_RES::OK
               : SCA_RES::ERR;
}

}  // namespace sca
