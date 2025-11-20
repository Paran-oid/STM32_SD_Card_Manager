#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

// it works only for relative paths
cmd_exec cd_exec = [](const cmd_args& args)
{
    const string& path = args[0];
    if (!fs::exists(path)) return status::err;
    if (!fs::is_directory(path)) return status::err;

    return fs::chdir(path);
};

}  // namespace stm_sd
