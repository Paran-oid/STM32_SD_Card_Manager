

#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec mkdirExec = [](const CmdArgs& args)
{
    if (args.empty()) return fail("args are not allowed to be empty");

    Status stat;
    for (const auto& arg : args)
    {
        if ((stat = fs::mkdir(arg)) != Status::OK) return stat;
    }

    return Status::OK;
};

CmdExec rmdirExec = [](const CmdArgs& args)
{
    if (args.empty()) return fail("args are not allowed to be empty");

    Status stat;
    for (const auto& arg : args)
    {
        if (!fs::exists(arg)) return Status::NO_FILE;
        if (!fs::isDirectory(arg)) return fail("only directories should be passed");
        if ((stat = fs::remove(arg, true)) != Status::OK) return stat;
    }

    return Status::OK;
};

}  // namespace stm_sd
