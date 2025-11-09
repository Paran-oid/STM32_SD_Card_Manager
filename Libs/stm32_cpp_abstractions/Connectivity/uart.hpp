#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/span.h>
#include <etl/string.h>

#include "defs.hpp"

namespace sca
{

class uart
{
   private:
    UART_HandleTypeDef& m_huart;

   public:
    uart() = delete;

    uart(UART_HandleTypeDef& huart) : m_huart {huart}
    {
    }

    ~uart() = default;

    SCA_RES send(etl::span<const uint8_t> tx);
    SCA_RES send(const uint8_t tx);

    SCA_RES receive(etl::span<uint8_t>& rx, bool wait_forever);
    SCA_RES receive(uint8_t& rx, bool wait_forever = true);

    template <size_t N>
    SCA_RES scan(etl::string<N>& out, bool display = true)
    {
        char   res[N];
        size_t size = 0;

        uint8_t c;

        for (;;)
        {
            if (this->receive(c) != SCA_RES::OK) return SCA_RES::ERR;

            if (c == '\r' || c == '\n')
            {
                res[size] = '\0';
                break;
            }

            if (size < N - 1)
            {
                res[size++] = static_cast<char>(c);
                if (display)
                {
                    if (this->send(c) != SCA_RES::OK) return SCA_RES::ERR;
                }
            }
        }

        out.assign(res);
        return SCA_RES::OK;
    }
};

}  // namespace sca
