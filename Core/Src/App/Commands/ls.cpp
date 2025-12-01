#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "sd_filesystem.hpp"
#include "utils.hpp"

namespace fs = stm_sd::sd_filesystem;

namespace stm_sd
{

CmdExec lsExec = [](const CmdArgs& args)
{
    string path;
    if (args.empty())
        path = ".";
    else
        path = args[0];

    etl::array<FILINFO, PAGE_SIZE> arr = {};

    uint8_t page = 0, res = 0;
    arr.assign(arr.begin(), arr.end());

    // res is returned number of files (number negative or null is an error/end of files)
    while ((res = fs::list(path, page, arr)) > 0)
    {
        for (uint8_t i = 0; i < res; i++)
        {
            FILINFO& item  = arr[i];
            bool     isDir = (item.fattrib & AM_DIR) != 0;
            printf_("%s%s\r\n", item.fname, isDir ? "/" : "");
        }
        page++;
    }

    return Status::OK;
};

}  // namespace stm_sd
