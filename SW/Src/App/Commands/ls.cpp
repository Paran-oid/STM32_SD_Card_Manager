#include <filesystem>

#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

namespace fs = std::filesystem;

CmdExec ls_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    etl::string<SSIZE> path;
    if (args.empty())
        path = "./";
    else
        path = args[0];

    fs::path dir_path(path.c_str());

    try
    {
        for (const auto& entry : fs::directory_iterator(dir_path))
        {
            etl::string<SSIZE> out;
            if (entry.is_regular_file())
            {
                out = entry.path().string().substr(2).c_str();
            }
            else if (entry.is_directory())
            {
                out = entry.path().string().substr(2).c_str();
                out += "/";
            }
            printf("%s  ", out.c_str());
        }
    }
    catch (const etl::string_exception& e)
    {
        printf("error...\n");
    }
    printf("\r\n");

    return SD_RES::OK;
};
