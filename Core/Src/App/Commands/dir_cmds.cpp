

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

cmd_exec mkdir_exec = [](const cmd_args& args)
{
    if (args.empty()) return fail("args are not allowed to be empty");

    status stat;
    for (const auto& arg : args)
    {
        if ((stat = fs::mkdir(arg)) != status::ok) return stat;
    }

    return status::ok;
};

cmd_exec rmdir_exec = [](const cmd_args& args)
{
    if (args.empty()) return fail("args are not allowed to be empty");

    status stat;
    for (const auto& arg : args)
    {
        if (!fs::exists(arg)) return status::no_file;
        if (!fs::is_directory(arg)) return fail("only directories should be passed");
        if ((stat = fs::remove(arg, true)) != status::ok) return stat;
    }

    return status::ok;
};

}  // namespace stm_sd
