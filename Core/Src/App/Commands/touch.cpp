#include "command_handler.hpp"
#include "sd_file.hpp"
#include "sd_filesystem.hpp"
#include "status.hpp"

namespace fs = stm_sd::sd_filesystem;

namespace stm_sd
{

CmdExec touchExec = [](const CmdArgs& args)
{
    /*
        Create an empty file
    */
    if (args.empty()) return Status::ERR;

    for (const auto& arg : args)
    {
        if (!isFilename(arg)) return Status::INVALID_NAME;
        SDFile* f = fs::open(arg, FCREATE_NEW);
        if (f) fs::close(f);
    }

    return Status::OK;
};

}  // namespace stm_sd
