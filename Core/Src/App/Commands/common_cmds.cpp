#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec clear_exec = [](const etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY>& args)
{
    (void) args;
    log("\033[3J\033[H\033[2J");
    return SD_RES::OK;
};

CmdExec pwd_exec = [](const etl::vector<etl::string<ARGS_BUF_ITEM_SIZE>, ARGS_BUF_CAPACITY>& args)
{
    (void) args;
    log(sd_reader.cwd());
    return SD_RES::OK;
};
