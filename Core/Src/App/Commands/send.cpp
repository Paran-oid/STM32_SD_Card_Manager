#include "command_handler.hpp"
#include "etl/to_arithmetic.h"
#include "hal_init.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

CmdExec sendExec = [](const CmdArgs& args)
{
    (void) args;
    return Status::OK;
};

}  // namespace stm_sd
