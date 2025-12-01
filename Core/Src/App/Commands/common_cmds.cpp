#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "printf.h"
#include "sd_filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::sd_filesystem;

namespace stm_sd
{
CmdExec clearExec = [](const CmdArgs& args)
{
    (void) args;
    printf_(CLEAR_CMD);
    return Status::OK;
};

CmdExec pwdExec = [](const CmdArgs& args)
{
    (void) args;
    printf_("%s\r\n", fs::cwd().c_str());
    return Status::OK;
};

CmdExec freeSpaceExec = [](const CmdArgs& args)
{
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::freeSpace()) / BYTES_PER_GB);
    return Status::OK;
};

CmdExec totalSpaceExec = [](const CmdArgs& args)
{
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::freeSpace()) / BYTES_PER_GB);
    return Status::OK;
};

}  // namespace stm_sd
