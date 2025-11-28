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

CmdExec catExec = [](const CmdArgs& args)
{
    if (args.empty()) return fail("args can't be empty");
    Status stat;

    // > and >> are only used for echo just for simplicity's sake
    auto writeSymb  = etl::find(args.begin(), args.end(), ">");
    auto appendSymb = etl::find(args.begin(), args.end(), ">>");

    bool    hasWrittenToFile = false;  // checks if we will output to a file or stdout (uart)
    File*   fres;
    uint8_t nbFilesToWrite = 0;
    if (writeSymb != args.end() && appendSymb != args.end())
        return fail(">> and > together are not allowed");
    else if (writeSymb != args.end() || appendSymb != args.end())
    {
        // find symbole >> or > index
        ptrdiff_t idxSymb =
            etl::distance(args.begin(), writeSymb != args.end() ? writeSymb : appendSymb);
        if (((idxSymb - 1) < 0) || (idxSymb + 1) > static_cast<ptrdiff_t>(args.size()))
            return Status::ERR;
        // example: cat file1 file2.txt > out.txt
        //               [0]   [1]     [2] [3]
        nbFilesToWrite = idxSymb;

        // assign flags for file
        uint8_t openMode = 0;
        if (writeSymb != args.end())
            openMode = FA_OPEN_ALWAYS | FA_WRITE;
        else if (appendSymb != args.end())
            openMode = FA_OPEN_APPEND | FA_WRITE;

        string outputFilePath = args[static_cast<size_t>(idxSymb + 1)];
        fres                    = fs::open(outputFilePath, openMode);
        if (!fres) return Status::ERR;
        if (writeSymb != args.end()) fres->truncate();  // set to start of file

        hasWrittenToFile = true;
    }

    for (const auto& arg : args)
    {
        if (arg == ">" || arg == ">>") break;
        string path = arg;

        if (!isFilename(path))
        {
            if (hasWrittenToFile) fs::close(fres);
            return fail("unvalid filename");
        }
        if (isDoubleQuoted(path)) path = formatStr(path);

        if (!fs::exists(path))
        {
            if (hasWrittenToFile) fs::close(fres);
            return Status::NO_FILE;
        }
        File* f = fs::open(path, FileMode::READ);
        if (!f) return Status::ERR;

        //* in reality we read BLOCK_SIZE - 1 chars at a time
        etl::string<BLOCK_SIZE> readBuf;
        if (hasWrittenToFile)
        {
            nbFilesToWrite--;
            while (f->read(readBuf)) fres->write(readBuf);
            if (nbFilesToWrite != 0) fres->write("\r\n");
        }
        else
        {
            while (f->read(readBuf)) printf_("%s", readBuf.c_str());
            printf_("\r\n");
        }

        if ((stat = fs::close(f)) != Status::OK) return stat;
    }

    if (hasWrittenToFile) return fs::close(fres);
    return Status::OK;
};

}  // namespace stm_sd
