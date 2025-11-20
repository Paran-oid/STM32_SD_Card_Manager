#include <fstream>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

CmdExec cat_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    etl::string<SSIZE> path = args[0];
    if (isstring(args[0])) path = format_str(path);

    std::ifstream f(path.c_str());  // TODO: use some kind of function that checks the existence of
                                    // a ;file instead of trying to open it...
    if (!f.is_open()) return SD_RES::ERR;

    std::string readbuf;  // have to use std::string to ready from getline
    while (std::getline(f, readbuf))
    {
        printf("%s\n", readbuf.c_str());
    }

    f.close();

    return SD_RES::OK;
};
