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

cmd_exec cat_exec = [](const cmd_args& args)
{
    if (args.empty()) return fail("args can't be empty");
    status stat;

    // > and >> are only used for echo just for simplicity's sake
    auto write_symb  = etl::find(args.begin(), args.end(), ">");
    auto append_symb = etl::find(args.begin(), args.end(), ">>");

    bool    out_to_file = false;  // checks if we will output to a file or stdout (uart)
    file*   fo;
    uint8_t files_to_write = 0;
    if (write_symb != args.end() && append_symb != args.end())
        return fail(">> and > together are not allowed");
    else if (write_symb != args.end() || append_symb != args.end())
    {
        // find symbole >> or > index
        ptrdiff_t idx_symb =
            etl::distance(args.begin(), write_symb != args.end() ? write_symb : append_symb);
        if (((idx_symb - 1) < 0) || (idx_symb + 1) > static_cast<ptrdiff_t>(args.size()))
            return status::err;
        // example: cat file1 file2.txt > out.txt
        //               [0]   [1]     [2] [3]
        files_to_write = idx_symb;

        // assign flags for file
        uint8_t open_mode = 0;
        if (write_symb != args.end())
            open_mode = FA_OPEN_ALWAYS | FA_WRITE;
        else if (append_symb != args.end())
            open_mode = FA_OPEN_APPEND | FA_WRITE;

        string output_file_path = args[static_cast<size_t>(idx_symb + 1)];
        fo                      = fs::open(output_file_path, open_mode);
        if (!fo) return status::err;
        if (write_symb != args.end()) fo->truncate();  // set to start of file

        out_to_file = true;
    }

    for (const auto& arg : args)
    {
        if (arg == ">" || arg == ">>") break;
        string path = arg;

        if (!is_filename(path))
        {
            if (out_to_file) fs::close(fo);
            return fail("unvalid filename");
        }
        if (is_double_quoted(path)) path = format_str(path);

        if (!fs::exists(path))
        {
            if (out_to_file) fs::close(fo);
            return status::no_file;
        }
        file* f = fs::open(path, file_mode::read);
        if (!f) return status::err;

        //* in reality we read BLOCK_SIZE - 1 chars at a time
        etl::string<BLOCK_SIZE> read_buf;
        if (out_to_file)
        {
            files_to_write--;
            while (f->read(read_buf)) fo->write(read_buf);
            if (files_to_write != 0) fo->write("\r\n");
        }
        else
        {
            while (f->read(read_buf)) printf("%s", read_buf.c_str());
            printf("\r\n");
        }

        if ((stat = fs::close(f)) != status::ok) return stat;
    }

    if (out_to_file) return fs::close(fo);
    return status::ok;
};

}  // namespace stm_sd
