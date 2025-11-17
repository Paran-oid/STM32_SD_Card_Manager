#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec ls_exec = [](const etl::vector<estring, ARGS_CAPACITY>& args)
{
    etl::string<SSIZE> path;
    if (args.empty())
        path = ".";
    else
        path = args[0];

    uint8_t                        page = 0;
    etl::array<FILINFO, PAGE_SIZE> arr;

    while (sd_reader.list(path, page, arr) == SD_RES::OK)
    {
        for (const auto& item : arr)
        {
            bool is_dir = (item.fattrib & AM_DIR) != 0;
            printf("%s%s\r\n", item.fname, is_dir ? "/" : "");
        }
        page++;
    }
    printf("\r\n");

    return SD_RES::OK;
};
