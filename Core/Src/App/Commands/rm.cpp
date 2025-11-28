#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec rmExec = [](const CmdArgs& args)
{
    if (args.empty()) return Status::ERR;

    Status stat;
    bool   isRecursive = false;
    for (const auto& arg : args)
    {
        if (isFlag(arg))
        {
            for (auto it = arg.begin() + 1; it != arg.end(); it++)
            {
                char c = *it;
                if (c == 'r') isRecursive = true;
            }
        }
    }

    for (const auto& arg : args)
    {
        if (isFlag(arg)) continue;
        if ((stat = fs::remove(arg, isRecursive)) != Status::OK) return stat;
    }

    return Status::OK;
};

}  // namespace stm_sd
