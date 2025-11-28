#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/vector.h>

#include "command_handler.hpp"
#include "file.hpp"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec echoExec = [](const CmdArgs& args)
{
    if (args.empty())
    {
        printf_("\r\n");
        return Status::OK;
    }

    Status stat;

    // > and >> are only used for echo just for simplicity's sake
    auto writeSymb  = etl::find(args.begin(), args.end(), ">");
    auto appendSymb = etl::find(args.begin(), args.end(), ">>");

    if (writeSymb != args.end() && appendSymb != args.end())
        return fail(">> and > together are not allowed");

    uint8_t openMode;
    if (writeSymb != args.end())
        openMode = FOPEN_ALWAYS | FWRITE;
    else if (appendSymb != args.end())
        openMode = FOPEN_APPEND | FWRITE;

    // just output content
    if (writeSymb == args.end() && appendSymb == args.end())
    {
        etl::string<SSIZE * CMD_HANDLER_ARGS_CAPACITY> outputStr;
        string temp;  // this gets modified and then added to outputStr
        for (auto it = args.begin(); it != args.end(); it++)
        {
            temp = *it;
            if (isDoubleQuoted(temp)) temp = formatStr(temp);
            outputStr += temp;
            if (etl::next(it) != args.end()) outputStr += " ";
        }
        printf_("%s\r\n", outputStr.c_str());
    }
    else
    {
        ptrdiff_t symbIdx =
            etl::distance(args.begin(), writeSymb != args.end() ? writeSymb : appendSymb);
        if (((symbIdx - 1) < 0) || (symbIdx + 1) > static_cast<ptrdiff_t>(args.size()))
            return Status::ERR;

        string content, temp;
        for (uint8_t i = 0; i < symbIdx; i++)
        {
            if (isDoubleQuoted(args[i]))
                temp = formatStr(args[i]);
            else
                temp = args[i];

            content += temp;
            if (i != (symbIdx - 1)) content += " ";
        }

        string outputFilePath = args[static_cast<size_t>(symbIdx + 1)];
        if (!isFilename(outputFilePath)) return Status::INVALID_NAME;
        File* file = fs::open(outputFilePath, openMode);
        if (!file) return Status::ERR;

        if (writeSymb != args.end()) file->truncate();  // set to start of file
        if ((stat = file->write(content)) != Status::OK) return stat;

        if ((stat = fs::close(file)) != Status::OK) return stat;
    }

    return Status::OK;
};

}  // namespace stm_sd
