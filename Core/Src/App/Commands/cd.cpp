#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

// it works only for relative paths
CmdExec cd_exec = [](const etl::vector<estring, ARGS_CAPACITY>& args)
{
    const estring& path = args[0];
    if (sd_reader.exists(path) != SD_RES::OK) return SD_RES::ERR;
    if (!sd_reader.is_directory(path)) return SD_RES::ERR;

    return sd_reader.chdir(path);
};
