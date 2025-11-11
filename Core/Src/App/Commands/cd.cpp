#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "utils.hpp"

CmdExec cd_exec = [](const etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY>& args)
{
    (void) args;
    return SD_RES::OK;
};
