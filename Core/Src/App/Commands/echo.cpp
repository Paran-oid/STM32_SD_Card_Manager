#include "command_handler.hpp"
#include "etl/algorithm.h"
#include "etl/string.h"
#include "etl/vector.h"
<<<<<<< HEAD
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec echo_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
=======
#include "utils.hpp"

CmdExec echo_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
>>>>>>> dc6478d (init)
{
    if (args.empty())
    {
        printf("\r\n");
        return SD_RES::OK;
    }

<<<<<<< HEAD
    // > and >> are only used for echo just for simplicity's sake
    auto write_symb  = etl::find(args.begin(), args.end(), ">");
    auto append_symb = etl::find(args.begin(), args.end(), ">>");

    if (write_symb != args.end() && append_symb != args.end())
        return SD_RES::ERR;  // both operators at same time are not allowed

    uint8_t open_mode;
    if (write_symb != args.end())
        open_mode = FA_OPEN_ALWAYS | FA_WRITE;
    else if (append_symb != args.end())
        open_mode = FA_OPEN_APPEND | FA_WRITE;

    // just write content
    if (write_symb == args.end() && append_symb == args.end())
    {
        etl::string<SSIZE * ARGS_CAPACITY> output_str;
=======
    auto write_symb  = etl::find(args.begin(), args.end(), ">");
    auto append_symb = etl::find(args.begin(), args.end(), ">>");

    if (write_symb == args.end() && append_symb == args.end())
    {
        etl::string<ARGS_ITEM_SIZE * ARGS_CAPACITY> output_str;
>>>>>>> dc6478d (init)
        for (auto it = args.begin(); it != args.end(); it++)
        {
            output_str += *it;
            if (etl::next(it) != args.end()) output_str += " ";
        }
<<<<<<< HEAD
        printf("%s", output_str.data());
    }
    else
    {
        int idx_symb =
            etl::distance(args.begin(), write_symb != args.end() ? write_symb : append_symb);
        if (((idx_symb - 1) < 0) || (idx_symb + 1) > static_cast<int>(args.size()))
            return SD_RES::ERR;

        etl::string<SSIZE> content = args[idx_symb - 1];

        etl::string<SSIZE> output_file = args[idx_symb + 1];
        SDFile*            file        = sd_reader.open_file(output_file, open_mode);
        if (write_symb) file->truncate();  // reduce size to the pointer if in write mode
        if (file->write(content) != SD_RES::OK) return SD_RES::ERR;

        if (sd_reader.close_file(file) != SD_RES::OK) return SD_RES::ERR;
    }
=======
        output_str += "\n";
        printf("%s", output_str.data());
    }
>>>>>>> dc6478d (init)

    return SD_RES::OK;
};
