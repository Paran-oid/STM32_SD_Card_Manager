#include "command_handler.hpp"
#include "filesystem.hpp"
#include "status.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

cmd_exec touch_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    for (const auto& arg : args)
    {
        file* f = fs::open(arg, FA_CREATE_NEW);
        if (f) fs::close(f);
    }

    return status::ok;
};

}  // namespace stm_sd
