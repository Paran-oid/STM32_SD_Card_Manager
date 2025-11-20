#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "file.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

extern "C"
{
#include "fatfs.h"
}

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

extern cmd_exec rm_exec;

cmd_exec cp_exec = [](const cmd_args& args)
{
    if (args.size() != 2) return fail("2 arguments must be passed");

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

    const string& src = args[0];
    const string& dst = args[1];
    if (!fs::exists(src)) return status::no_file;

    if (fs::is_directory(dst))
    {
        bool is_src_dir = fs::is_directory(src);

        uint8_t opts = OVERWRITE;
        if (is_src_dir) opts |= RECURSIVE;

        fs::copy(src, dst, opts);  // copies into a directory the file or even
                                   // the directory in question
    }
    else
    {
        if (fs::is_directory(src) || !is_recursive)
            return fail(
                "recursive flag needed to copy directories");  // can't copy directory into a file
                                                               // and/or recursive flag is needed
        fs::copy(src, dst, OVERWRITE);
    }
    return status::ok;
};

}  // namespace stm_sd
