#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec rm_exec = [](const etl::vector<string, ARGS_CAPACITY>& args)
{
    if (args.empty()) return StatusCode::ERR;

    for (const auto& arg : args)
    {
        // TODO: start handling flags
        if (fs::is_directory(arg)) continue;
        if (fs::remove(arg) != StatusCode::OK) return StatusCode::ERR;
    }

    return StatusCode::OK;
};

}  // namespace stm_sd
