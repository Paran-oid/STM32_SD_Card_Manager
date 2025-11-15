#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec rm_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    // TODO: start handling flags!
    if (args.empty()) return SD_RES::ERR;

    for (const auto& arg : args)
    {
        if (fs::is_directory(arg.c_str()))
            continue;
        else
        {
            if (!fs::remove(arg.c_str())) return SD_RES::ERR;
        }
    }

    return SD_RES::OK;
};
