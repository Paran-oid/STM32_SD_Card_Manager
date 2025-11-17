

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec mkdir_exec = [](const etl::vector<estring, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    for (const auto& arg : args)
    {
        if (sd_reader.mkdir(arg) != SD_RES::OK) return SD_RES::ERR;
    }

    return SD_RES::OK;
};

CmdExec rmdir_exec = [](const etl::vector<estring, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    for (const auto& arg : args)
    {
        if (sd_reader.delete_(arg, true) != SD_RES::OK) return SD_RES::ERR;
    }

    return SD_RES::OK;
};
