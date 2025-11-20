#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec cp_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty() || args.size() != 2) return SD_RES::ERR;

    const etl::string<SSIZE>& src = args[0];
    const etl::string<SSIZE>& dst = args[1];
    if (!fs::exists(src.c_str())) return SD_RES::ERR;

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
            return SD_RES::ERR;  // can't copy directory into a file???
        fs::copy_file(src.c_str(), dst.c_str(), fs::copy_options::overwrite_existing);
    }

    return SD_RES::OK;
};
