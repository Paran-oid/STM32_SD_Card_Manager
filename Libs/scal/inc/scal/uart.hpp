#pragma once

extern "C"
{
#include "main.h"
}

#include <etl/span.h>
#include <etl/string.h>

#include "defs.hpp"

namespace scal
{

#ifdef SCA_UART

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

    SCA_RES send(etl::string_view str);
    SCA_RES send(etl::span<const uint8_t> tx);
    SCA_RES send(const uint8_t tx);

    template <size_t N>
    SCA_RES receive(etl::string<N>& str, bool wait_forever)
    {
        char buf[N];
        if (HAL_UART_Receive(&m_huart, buf, str.size(), wait_forever ? HAL_MAX_DELAY : 100) !=
            HAL_OK)
        {
            return SCA_RES::ERR;
        }

        str.assign(buf);
        return SCA_RES::OK;
    }
    SCA_RES receive(etl::span<uint8_t>& rx, bool wait_forever = true);
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
                this->send("\r\n");
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

#endif

}  // namespace scal
