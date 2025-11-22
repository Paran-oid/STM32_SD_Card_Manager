#include <filesystem>

#include "command_handler.hpp"
#include "etl/vector.h"
#include "status.hpp"

namespace fs = std::filesystem;

namespace stm_sd
{

cmd_exec ls_exec = [](const cmd_args& args)
{
    string path;
    if (args.empty())
        path = "./";
    else
        path = args[0];

    fs::path dir_path(path.c_str());

    try
    {
        for (const auto& entry : fs::directory_iterator(dir_path))
        {
            string out;
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

    return status::ok;
};

}  // namespace stm_sd
