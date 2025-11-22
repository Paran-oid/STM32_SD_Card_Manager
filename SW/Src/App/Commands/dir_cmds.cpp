

#include <filesystem>

#include "command_handler.hpp"
#include "status.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec mkdir_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    for (const auto& arg : args)
    {
        if (!fs::create_directories(arg.c_str())) return status::err;
    }

    return status::ok;
};

cmd_exec rmdir_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    for (const auto& arg : args)
    {
        if (!fs::remove_all(arg.c_str())) return status::err;
    }

    return status::ok;
};

}  // namespace stm_sd
