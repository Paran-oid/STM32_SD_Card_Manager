#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/bit.h>
#include <etl/span.h>

#include "utils.hpp"

// TODO: test this peripheral
class SPI
{
   private:
    SPI_HandleTypeDef& m_hspi;

   public:
    SPI() = delete;

    SPI(SPI_HandleTypeDef& hspi) : m_hspi {hspi}
    {
    }

    ~SPI() = default;

    MP_RES send(etl::span<const uint8_t> tx);
    MP_RES send(const uint8_t tx);

    MP_RES receive(etl::span<uint8_t>& rx, bool wait_forever = true);
    MP_RES receive(uint8_t& rx, bool wait_forever = true);

    MP_RES send_and_receive(etl::span<const uint8_t> tx, etl::span<uint8_t>& rx);
};
