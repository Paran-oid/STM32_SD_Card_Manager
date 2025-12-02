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
    /*
        Clears content of the console
    */

    (void) args;
    printf_("\033[2J\033[H\r\n");  // clear escape sequence for "cleaning" the stdout
    return Status::OK;
};

CmdExec pwdExec = [](const CmdArgs& args)
{
    /*
        Prints the current working directory
    */
    (void) args;
    printf_("%s\r\n", fs::cwd().c_str());
    return Status::OK;
};

CmdExec freeSpaceExec = [](const CmdArgs& args)
{
    /*
        Displays the free space left
    */
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::freeSpace()) / BYTES_PER_GB);
    return Status::OK;
};

CmdExec totalSpaceExec = [](const CmdArgs& args)
{
    /*
        Displays the total space
    */
    (void) args;
    printf_("%.2fGB\r\n", static_cast<double>(fs::freeSpace()) / BYTES_PER_GB);
    return Status::OK;
};

}  // namespace stm_sd
