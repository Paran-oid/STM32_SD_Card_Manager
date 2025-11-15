#include <fstream>

#include "command_handler.hpp"
#include "etl/algorithm.h"
#include "etl/string.h"
#include "etl/vector.h"
#include "utils.hpp"

CmdExec echo_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
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

    // just print content
    if (write_symb == args.end() && append_symb == args.end())
    {
        etl::string<SSIZE * ARGS_CAPACITY> output_str;
        etl::string<SSIZE>                 temp;

        for (auto it = args.begin(); it != args.end(); it++)
        {
            temp = *it;
            if (is_double_quoted(temp))
            {
                temp = format_str(temp);  // get rid of double quotes for the string
            }

            output_str += temp;
            if (etl::next(it) != args.end()) output_str += " ";
        }
        printf("%s\r\n", output_str.data());
    }
    else
    {
        ptrdiff_t idx_symb =
            etl::distance(args.begin(), write_symb != args.end() ? write_symb : append_symb);
        if ((idx_symb - 1) < 0 || (idx_symb + 1) > static_cast<ptrdiff_t>(args.size()))
            return SD_RES::ERR;

        etl::string<SSIZE> content, temp;
        for (uint8_t i = 0; i < idx_symb; i++)
        {
            if (is_double_quoted(args[i]))
                temp = format_str(args[i]);
            else
                temp = args[i];

            content += temp;
            if (i != (idx_symb - 1)) content += " ";
        }

        etl::string<SSIZE> output_file = args[static_cast<size_t>(idx_symb + 1)];

        std::ofstream file(output_file.data(),
                           append_symb != args.end() ? std::ios::app : std::ios::out);
        if (!file.is_open()) return SD_RES::ERR;

        file << content.data();

        file.close();
    }

    return SD_RES::OK;
};
