#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
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
    printf_("\033[2J\033[H\r\n");
    return status::ok;
};

cmd_exec pwd_exec = [](const cmd_args& args)
{
    (void) args;
    printf_("%s\r\n", fs::cwd().c_str());
    return status::ok;
};

cmd_exec free_space_exec = [](const cmd_args& args)
{
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::free_space()) / BYTES_PER_GB);
    return status::ok;
};

cmd_exec total_space_exec = [](const cmd_args& args)
{
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::free_space()) / BYTES_PER_GB);
    return status::ok;
};

}  // namespace stm_sd
