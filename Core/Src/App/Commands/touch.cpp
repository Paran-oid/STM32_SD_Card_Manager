#include "command_handler.hpp"
#include "sd.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec touch_exec = [](const CmdArgs& args)
{
    if (args.empty()) return StatusCode::ERR;

    for (const auto& arg : args)
    {
        File* f = fs::open(arg, FA_CREATE_NEW);
        if (f) fs::close(f);
    }

    return StatusCode::OK;
};

}  // namespace stm_sd
