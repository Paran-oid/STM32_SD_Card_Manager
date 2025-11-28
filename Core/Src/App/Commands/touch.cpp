#include "command_handler.hpp"
#include "filesystem.hpp"
#include "status.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec touchExec = [](const CmdArgs& args)
{
    if (args.empty()) return Status::ERR;

    for (const auto& arg : args)
    {
        if (!isFilename(arg)) return Status::INVALID_NAME;
        File* f = fs::open(arg, FCREATE_NEW);
        if (f) fs::close(f);
    }

    return Status::OK;
};

}  // namespace stm_sd
