#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

CmdExec cat_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    const etl::string<SSIZE>& path = args[0];
    if (sd_reader.exists(path) != SD_RES::OK) return SD_RES::ERR;

    SDFile* f = sd_reader.open_file(path, FA_READ);
    if (!f) return SD_RES::ERR;

    etl::string<SSIZE> read_buf;
    while (f->read(read_buf) != SD_RES::ERR) printf("%s", read_buf.data());

    return SD_RES::OK;
};
