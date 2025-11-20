#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

cmd_exec rm_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    status stat;
    bool   is_recursive = false;
    for (const auto& arg : args)
    {
        if (is_flag(arg))
        {
            for (auto it = arg.begin() + 1; it != arg.end(); it++)
            {
                char c = *it;
                if (c == 'r') is_recursive = true;
            }
        }
    }

    for (const auto& arg : args)
    {
        if (is_flag(arg)) continue;
        if ((stat = fs::remove(arg, is_recursive)) != status::ok) return stat;
    }

    return status::ok;
};

}  // namespace stm_sd
