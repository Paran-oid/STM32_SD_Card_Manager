#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "printf.h"
#include "utils.hpp"

<<<<<<< HEAD
CmdExec clear_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
=======
CmdExec clear_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
>>>>>>> dc6478d (init)
{
    (void) args;
    printf("\033[2J\033[H\r\n");
    return SD_RES::OK;
};

<<<<<<< HEAD
CmdExec pwd_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
=======
CmdExec pwd_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
>>>>>>> dc6478d (init)
{
    (void) args;
    printf("%s\r\n", sd_reader.cwd().data());
    return SD_RES::OK;
};
