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

    for (const auto& arg : args)
    {
        status stat;
        string path = arg;

        if (!is_filename(path)) return fail("unvalid filename");
        if (is_double_quoted(path)) path = format_str(path);

        if (!fs::exists(path)) return status::no_file;
        file* f = fs::open(path, file_mode::read);
        if (!f) return status::err;

        //* in reality we read BLOCK_SIZE - 1 chars at a time
        etl::string<BLOCK_SIZE> read_buf;
        while (f->read(read_buf)) printf("%s", read_buf.c_str());
        printf("\r\n");

        if ((stat = fs::close(f)) != status::ok) return stat;
    }

    return status::ok;
};

}  // namespace stm_sd
