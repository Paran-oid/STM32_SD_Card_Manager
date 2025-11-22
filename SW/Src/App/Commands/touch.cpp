#include <fstream>

#include "command_handler.hpp"
#include "status.hpp"

namespace stm_sd
{

cmd_exec touch_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    for (const auto& arg : args)
    {
        std::ofstream f(arg.c_str());
    }

    return status::ok;
};

}  // namespace stm_sd
