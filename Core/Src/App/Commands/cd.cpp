#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

// it works only for relative paths
CmdExec cd_exec = [](const CmdArgs& args)
{
    const string& path = args[0];
    if (!fs::exists(path)) return StatusCode::ERR;
    if (!fs::is_directory(path)) return StatusCode::ERR;

    return fs::chdir(path);
};

}  // namespace stm_sd
