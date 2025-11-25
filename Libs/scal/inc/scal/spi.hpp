#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/bit.h>
#include <etl/span.h>

#include "defs.hpp"

namespace scal
{

#ifdef SCA_SPI

class spi
{
   private:
    SPI_HandleTypeDef& m_hspi;

   public:
    spi() = delete;

    spi(SPI_HandleTypeDef& hspi) : m_hspi {hspi}
    {
    }

    ~spi() = default;

    SCA_RES send(etl::span<const uint8_t> tx);
    SCA_RES send(const uint8_t tx);

    SCA_RES receive(etl::span<uint8_t>& rx, bool wait_forever = true);
    SCA_RES receive(uint8_t& rx, bool wait_forever = true);

    SCA_RES send_and_receive(etl::span<const uint8_t> tx, etl::span<uint8_t>& rx);
};

#endif

}  // namespace scal
