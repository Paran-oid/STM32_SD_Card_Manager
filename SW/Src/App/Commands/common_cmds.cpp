
#include "command_handler.hpp"
#include "status.hpp"

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
    // printf("%s\r\n", sd_reader.cwd().c_str()); // already exists in stm32 code
    printf("%s\r\n", g_cwd.c_str());
    return status::ok;
};

}  // namespace stm_sd
