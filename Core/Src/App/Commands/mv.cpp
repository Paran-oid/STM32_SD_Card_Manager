#include "command_handler.hpp"
#include "filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

cmd_exec mv_exec = [](const cmd_args& args)
{
    // renames a file
    // TODO: add other features related to mv like moving a dir into another and etc

    if (args.size() != 2) return fail("unallowed to pass more than 2 args");

    const string& new_name = args.back();
    const string& old_name = args.front();

    if (fs::is_directory(new_name) || fs::is_directory(old_name))
        return fail("unallowed to enter two directories");

    const PathData p = extract_path(new_name);
    if (!p.folder.empty() && !fs::exists(p.folder)) fs::mkdir(p.folder);

    return fs::rename(old_name, new_name);
};

}  // namespace stm_sd
