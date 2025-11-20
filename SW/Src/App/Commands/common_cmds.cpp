#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec clear_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    printf("\033[2J\033[H\r\n");
    return SD_RES::OK;
};

CmdExec pwd_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    (void) args;
    // printf("%s\r\n", sd_reader.cwd().c_str()); // already exists in stm32 code
    printf("%s\r\n", g_cwd.c_str());
    return SD_RES::OK;
};
