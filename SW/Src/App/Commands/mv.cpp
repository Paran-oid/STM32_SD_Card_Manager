#include <filesystem>

#include "command_handler.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec mv_exec = [](const cmd_args& args)
{
    // renames a file
    // TODO: add other features related to mv like moving a dir into another and etc

    if (args.size() != 2) return fail("unallowed to pass more than 2 args");

    const string& new_name = args.back();
    const string& old_name = args.front();

    if (fs::is_directory(new_name.c_str()) || fs::is_directory(old_name.c_str()))
        return fail("unallowed to enter two directories");

    const PathData p = extract_path(new_name);
    if (!p.folder.empty() && !fs::exists(p.folder.c_str())) fs::create_directory(p.folder.c_str());

    fs::rename(old_name.c_str(), new_name.c_str());
    return status::ok;
};

}  // namespace stm_sd
