#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "status.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec rm_exec = [](const cmd_args& args)
{
    // TODO: start handling flags!
    if (args.empty()) return status::err;

    for (const auto& arg : args)
    {
        if (fs::is_directory(arg.c_str())) continue;
        if (!fs::remove(arg.c_str())) return status::err;
    }

    return status::ok;
};

}  // namespace stm_sd
