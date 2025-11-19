

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec mkdir_exec = [](const CmdArgs& args)
{
    if (args.empty()) return StatusCode::ERR;

    for (const auto& arg : args)
    {
        if (fs::mkdir(arg) != StatusCode::OK) return StatusCode::ERR;
    }

    return StatusCode::OK;
};

CmdExec rmdir_exec = [](const CmdArgs& args)
{
    if (args.empty()) return StatusCode::ERR;

    for (const auto& arg : args)
    {
        if (!fs::exists(arg)) return StatusCode::ERR;
        if (!fs::is_directory(arg)) return StatusCode::ERR;
        if (fs::remove(arg, true) != StatusCode::OK) return StatusCode::ERR;
    }

    return StatusCode::OK;
};

}  // namespace stm_sd
