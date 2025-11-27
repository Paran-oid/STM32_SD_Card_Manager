#include "command_handler.hpp"
#include "etl/to_arithmetic.h"
#include "hal_init.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

#define DEFAULT_RECEIVE_TIMEOUT_SECONDS 5

extern cmd_exec clear_exec;

cmd_exec receive_exec = [](const cmd_args& args)
{
    // TODO: make -s flag for number of seconds to wait
    uint32_t timeout_ms;
    if (args.empty())
        timeout_ms = DEFAULT_RECEIVE_TIMEOUT_SECONDS * 1000;
    else
    {
        etl::to_arithmetic_result cres = etl::to_arithmetic<uint32_t>(args[0]);
        if (cres.error() != etl::to_arithmetic_status::Valid) return status::invalid_parameter;

        timeout_ms = etl::to_arithmetic<uint32_t>(args[0]).value() * 1000;
    }

    clear_exec({});
    printf_("### WAITING FOR DATA TO BE SENT! ###\r\n");

    char c;
    do
    {
        auto res = HAL_UART_Receive(&huart2, (uint8_t*) &c, 1, timeout_ms);
        if (res != HAL_OK) return fail("didn't receive anything...");
        printf_("%#X ", c);
        c = 0;  // reset
    } while (true);

    return status::ok;
};

}  // namespace stm_sd
