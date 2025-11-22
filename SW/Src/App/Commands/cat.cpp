#include <fstream>

#include "command_handler.hpp"
#include "etl/vector.h"
#include "utils.hpp"

namespace stm_sd
{

cmd_exec cat_exec = [](const cmd_args& args)
{
    if (args.empty()) return status::err;

    string path = args[0];
    if (is_double_quoted(args[0])) path = format_str(path);

    std::ifstream f(path.c_str());  // TODO: use some kind of function that checks the existence of
                                    // a ;file instead of trying to open it...
    if (!f.is_open()) return status::err;

    std::string readbuf;  // have to use std::string to ready from getline
    while (std::getline(f, readbuf))
    {
        printf("%s\n", readbuf.c_str());
    }

    f.close();

    return status::ok;
};

}  // namespace stm_sd
