
#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec mkdir_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    for (const auto& arg : args)
    {
        if (!fs::create_directories(arg.c_str())) return SD_RES::ERR;
    }

    return SD_RES::OK;
};

CmdExec rmdir_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    for (const auto& arg : args)
    {
        if (!fs::remove_all(arg.c_str())) return SD_RES::ERR;
    }

    return SD_RES::OK;
};
