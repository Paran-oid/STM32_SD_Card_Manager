#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec cd_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty() || args.size() > 1) return SD_RES::ERR;

    const etl::string<SSIZE>& arg  = args[0];
    const fs::path            path = fs::path(arg.c_str());
    if (!fs::exists(path)) return SD_RES::ERR;
    if (!fs::is_directory(path)) return SD_RES::ERR;

    g_cwd.assign(path.string().c_str());
    fs::current_path(path);

    return SD_RES::OK;
};
