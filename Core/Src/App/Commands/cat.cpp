#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

CmdExec cat_exec = [](const etl::vector<string, ARGS_CAPACITY>& args)
{
    if (args.empty()) return StatusCode::ERR;

    string path = args[0];
    if (is_double_quoted(path)) path = format_str(path);

    if (!fs::exists(path)) return StatusCode::ERR;
    File* f = fs::open(path, FA_READ);
    if (!f) return StatusCode::ERR;

    etl::string<BLOCK_SIZE> read_buf;
    while (f->read(read_buf)) printf("%s", read_buf.c_str());
    printf("\r\n");

    if (fs::close(f) != StatusCode::OK) return StatusCode::ERR;

    return StatusCode::OK;
};

}  // namespace stm_sd
