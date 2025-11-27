#include "command_handler.hpp"
#include "etl/to_arithmetic.h"
#include "hal_init.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

cmd_exec send_exec = [](const cmd_args& args)
{
    (void) args;
    return status::ok;
};

}  // namespace stm_sd
