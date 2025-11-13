#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

<<<<<<< HEAD
CmdExec ls_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
=======
CmdExec ls_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
>>>>>>> dc6478d (init)
{
    (void) args;
    return SD_RES::OK;
};
