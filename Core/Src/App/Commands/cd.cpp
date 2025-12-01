#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "sd_filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::sd_filesystem;

namespace stm_sd
{

// it works only for relative paths
CmdExec cdExec = [](const CmdArgs& args)
{
    const string& path = args[0];
    if (!fs::exists(path)) return fail("path doesn't exist...");
    if (!fs::isDirectory(path)) return fail("can't cd to a file");

    return fs::chdir(path);
};

}  // namespace stm_sd
