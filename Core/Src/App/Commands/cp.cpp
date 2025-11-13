#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

CmdExec cp_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    return SD_RES::OK;
};
