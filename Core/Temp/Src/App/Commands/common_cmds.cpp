#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "printf.h"
#include "utils.hpp"

CmdExec clear_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("%s", "\033[3J\033[H\033[2J");
    return SD_RES::OK;
};

CmdExec pwd_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("%s", sd_reader.cwd().data());
    return SD_RES::OK;
};
