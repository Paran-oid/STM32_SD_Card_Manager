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
    // printf("%s\r\n", sd_reader.cwd().data()); // already exists in stm32 code
    fs::path cwd = fs::current_path();
    printf("%s\r\n", cwd.string().data());
    return SD_RES::OK;
};
