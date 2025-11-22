#include <filesystem>

#include "command_handler.hpp"
#include "defs.hpp"
#include "etl/vector.h"
#include "status.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec cd_exec = [](const cmd_args& args)
{
    if (args.empty() || args.size() > 1) return status::err;

    const string&  arg  = args[0];
    const fs::path path = fs::path(arg.c_str());
    if (!fs::exists(path)) return status::err;
    if (!fs::is_directory(path)) return status::err;

    g_cwd.assign(path.string().c_str());
    fs::current_path(path);

    return status::ok;
};

}  // namespace stm_sd
