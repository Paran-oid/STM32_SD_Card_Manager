#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec cat_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    etl::string<SSIZE> path = args[0];
    if (is_double_quoted(path)) path = format_str(path);

    if (sd_reader.exists(path) != SD_RES::OK) return SD_RES::ERR;
    SDFile* f = sd_reader.open_file(path, FA_READ);
    if (!f) return SD_RES::ERR;

    etl::string<BLOCK_SIZE> read_buf;
    while (f->read(read_buf) != SD_RES::ERR) printf("%s", read_buf.data());
    printf("\r\n");

    if (sd_reader.close_file(f) != SD_RES::OK) return SD_RES::ERR;

    return SD_RES::OK;
};
