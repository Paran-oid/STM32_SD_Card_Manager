#include "command_handler.hpp"
#include "etl/algorithm.h"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

CmdExec echo_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty())
    {
        printf("\r\n");
        return SD_RES::OK;
    }

    auto write_symb  = etl::find(args.begin(), args.end(), ">");
    auto append_symb = etl::find(args.begin(), args.end(), ">>");

    if (write_symb == args.end() && append_symb == args.end())
    {
        etl::string<ARGS_ITEM_SIZE * ARGS_CAPACITY> output_str;
        for (auto it = args.begin(); it != args.end(); it++)
        {
            output_str += *it;
            if (etl::next(it) != args.end()) output_str += " ";
        }
        output_str += "\n";
        printf("%s", output_str.data());
    }

    return SD_RES::OK;
};
