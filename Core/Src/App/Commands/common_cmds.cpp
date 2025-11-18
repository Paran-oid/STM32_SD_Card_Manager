#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "printf.h"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{
CmdExec clear_exec = [](const etl::vector<string, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("\033[2J\033[H\r\n");
    return StatusCode::OK;
};

CmdExec pwd_exec = [](const etl::vector<string, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("%s\r\n", fs::cwd().c_str());
    return StatusCode::OK;
};

}  // namespace stm_sd
