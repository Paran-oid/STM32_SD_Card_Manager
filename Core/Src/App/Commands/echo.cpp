#include "command_handler.hpp"
#include "etl/algorithm.h"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec echo_exec = [](const etl::vector<estring, ARGS_CAPACITY>& args)
{
    if (args.empty())
    {
        printf("\r\n");
        return SD_RES::OK;
    }

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
        estring                            temp;
        for (auto it = args.begin(); it != args.end(); it++)
        {
            temp = *it;
            if (is_double_quoted(temp)) temp = format_str(temp);
            output_str += temp;
            if (etl::next(it) != args.end()) output_str += " ";
        }
        printf("%s\r\n", output_str.c_str());
    }
    else
    {
        ptrdiff_t idx_symb =
            etl::distance(args.begin(), write_symb != args.end() ? write_symb : append_symb);
        if (((idx_symb - 1) < 0) || (idx_symb + 1) > static_cast<ptrdiff_t>(args.size()))
            return SD_RES::ERR;

        estring content, temp;
        for (uint8_t i = 0; i < idx_symb; i++)
        {
            if (is_double_quoted(args[i]))
                temp = format_str(args[i]);
            else
                temp = args[i];

            content += temp;
            if (i != (idx_symb - 1)) content += " ";
        }

        estring output_file = args[static_cast<size_t>(idx_symb + 1)];
        SDFile* file        = sd_reader.open_file(output_file, open_mode);

        if (write_symb) file->truncate();  // set to start of file
        if (file->write(content) != SD_RES::OK) return SD_RES::ERR;

        if (sd_reader.close_file(file) != SD_RES::OK) return SD_RES::ERR;
    }

    return SD_RES::OK;
};
