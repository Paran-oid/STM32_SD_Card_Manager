#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "printf.h"
#include "utils.hpp"

CmdExec clear_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("\033[2J\033[H\r\n");
    return SD_RES::OK;
};

CmdExec pwd_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("%s\r\n", sd_reader.cwd().data());
    return SD_RES::OK;
};
