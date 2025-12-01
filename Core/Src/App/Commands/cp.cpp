#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "sd_file.hpp"
#include "sd_filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::sd_filesystem;

namespace stm_sd
{

CmdExec cpExec = [](const CmdArgs& args)
{
    if (args.size() != 2) return fail("2 arguments must be passed");

    bool isRecursive = false;
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

    const string& src = args[0];
    const string& dst = args[1];
    if (!fs::exists(src)) return Status::NO_FILE;

    if (fs::isDirectory(dst))
    {
        bool isSrcDir = fs::isDirectory(src);
        if (isSrcDir && !isRecursive) return fail("-r must be added for copying directories");

        uint8_t opts = OVERWRITE;
        if (isSrcDir)
        {
            opts |= RECURSIVE;
        }
        else
        {
            if (!isFilename(src)) return Status::INVALID_NAME;
        }

        fs::copy(src, dst, opts);  // copies into a directory the file or even
                                   // the directory in question
    }
    else
    {
        if (fs::isDirectory(src)) return fail("impossible to copy a directory into a file");
        fs::copy(src, dst, OVERWRITE);
    }
    return Status::OK;
};

}  // namespace stm_sd
