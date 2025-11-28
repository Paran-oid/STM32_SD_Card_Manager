#include "command_handler.hpp"
#include "filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec mvExec = [](const CmdArgs& args)
{
    if (args.size() != 2) return fail("unallowed to pass more than 2 args");

    const string& newName = args.back();
    const string& oldName = args.front();

    if (fs::isDirectory(newName) || fs::isDirectory(oldName))
        return fail("unallowed to enter two directories");

    const PathData p = extractPath(newName);
    if (!p.folder.empty() && !fs::exists(p.folder)) fs::mkdir(p.folder);

    return fs::rename(oldName, newName);
};

}  // namespace stm_sd
