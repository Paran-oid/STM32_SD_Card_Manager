#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec ls_exec = [](const CmdArgs& args)
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
            FILINFO& item   = arr[i];
            bool     is_dir = (item.fattrib & AM_DIR) != 0;
            printf("%s%s\r\n", item.fname, is_dir ? "/" : "");
        }
        page++;
    }

    return StatusCode::OK;
};

}  // namespace stm_sd
