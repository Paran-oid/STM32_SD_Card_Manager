#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec rm_exec = [](const CmdArgs& args)
{
    if (args.empty()) return StatusCode::ERR;

    bool is_recursive = false;
    for (const auto& arg : args)
    {
        if (is_flag(arg))
        {
            for (auto it = arg.begin() + 1; it != arg.end(); it++)
            {
                char c = *it;
                if (c == 'r') is_recursive = true;
            }
        }
    }

    for (const auto& arg : args)
    {
        if (is_flag(arg)) continue;
        if (fs::remove(arg, is_recursive) != StatusCode::OK) return StatusCode::ERR;
    }

    return StatusCode::OK;
};

}  // namespace stm_sd
