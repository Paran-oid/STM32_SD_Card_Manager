#include "command_handler.hpp"
#include "etl/to_arithmetic.h"
#include "hal_init.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

#define DEFAULT_RECEIVE_TIMEOUT_SECONDS 5

CmdExec receiveExec = [](const CmdArgs& args)
{
    // TODO: make -s flag for number of seconds to wait
    uint32_t timeoutMS;  // whenever we multiply by 1000 it is to turn it from mseconds into seconds
    if (args.empty())
        timeoutMS = DEFAULT_RECEIVE_TIMEOUT_SECONDS * 1000;
    else
    {
        etl::to_arithmetic_result cres = etl::to_arithmetic<uint32_t>(args[0]);
        if (cres.error() != etl::to_arithmetic_status::Valid) return Status::INVALID_PARAMETER;

        timeoutMS = etl::to_arithmetic<uint32_t>(args[0]).value() * 1000;
    }

    printf_(CLEAR_CMD);
    printf_("### WAITING FOR DATA TO BE SENT! ###\r\n");

    char c;
    do
    {
        auto res = HAL_UART_Receive(&huart2, (uint8_t*) &c, 1, timeoutMS);
        if (res != HAL_OK) return fail("didn't receive anything...");
        printf_("%#X ", c);
        c = 0;  // reset
    } while (true);

    return Status::OK;
};

}  // namespace stm_sd
