#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/span.h>
#include <etl/string.h>

#include <utils.hpp>

class UART
{
   private:
    UART_HandleTypeDef& m_huart;

   public:
    UART() = delete;

    UART(UART_HandleTypeDef& huart) : m_huart {huart}
    {
    }

    ~UART() = default;

    MP_RES send(etl::span<const uint8_t> tx);
    MP_RES send(const uint8_t tx);

    MP_RES receive(etl::span<uint8_t>& rx, bool wait_forever);
    MP_RES receive(uint8_t& rx, bool wait_forever = true);

    template <size_t N>
    MP_RES scan(etl::string<N>& out, bool display = true)
    {
        char   res[N];
        size_t size = 0;

        uint8_t c;

        for (;;)
        {
            if (this->receive(c) != MP_RES::OK) return MP_RES::ERR;

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
                    if (this->send(c) != MP_RES::OK) return MP_RES::ERR;
                }
            }
        }

        out.assign(res);
        return MP_RES::OK;
    }
};
