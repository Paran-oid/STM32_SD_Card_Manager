#include <filesystem>

#include "command_handler.hpp"
#include "etl/vector.h"
#include "status.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec cp_exec = [](const cmd_args& args)
{
    if (args.empty() || args.size() != 2) return status::err;

    const string& src = args[0];
    const string& dst = args[1];
    if (!fs::exists(src.c_str())) return status::err;

    if (fs::is_directory(dst.c_str()))
    {
        bool is_src_dir = fs::is_directory(src.c_str());

        auto opts = fs::copy_options::overwrite_existing;
        if (is_src_dir) opts |= fs::copy_options::recursive;

        fs::copy(src.c_str(), dst.c_str(), opts);  // copies into a directory the file or even
                                                   // the directory in question
    }
    else
    {
        if (fs::is_directory(src.c_str()))
            return status::err;  // can't copy directory into a file???
        fs::copy_file(src.c_str(), dst.c_str(), fs::copy_options::overwrite_existing);
    }

    return status::ok;
};

}  // namespace stm_sd
