#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "printf.h"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{
cmd_exec clear_exec = [](const cmd_args& args)
{
    (void) args;
    printf("\033[2J\033[H\r\n");
    return status::ok;
};

cmd_exec pwd_exec = [](const cmd_args& args)
{
    (void) args;
    printf("%s\r\n", fs::cwd().c_str());
    return status::ok;
};

}  // namespace stm_sd
