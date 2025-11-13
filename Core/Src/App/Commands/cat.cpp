#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

<<<<<<< HEAD
CmdExec cat_exec = [](const etl::vector<etl::string<SSIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    const etl::string<SSIZE>& path = args[0];
=======
CmdExec cat_exec = [](const etl::vector<etl::string<ARGS_ITEM_SIZE>, ARGS_CAPACITY>& args)
{
    if (args.empty()) return SD_RES::ERR;

    const etl::string<MAX_DIR_SIZE>& path = args[0];
>>>>>>> dc6478d (init)
    if (sd_reader.exists(path) != SD_RES::OK) return SD_RES::ERR;

    SDFile* f = sd_reader.open_file(path, FA_READ);
    if (!f) return SD_RES::ERR;

<<<<<<< HEAD
    etl::string<SSIZE> read_buf;
    while (f->read(read_buf) != SD_RES::ERR) printf("%s", read_buf.data());

=======
    etl::string<100> read_buf;
    while (f->read(read_buf) != SD_RES::ERR) printf("%s", read_buf.data());  // TODO: test this
    printf("\r\n");

    (void) args;
>>>>>>> dc6478d (init)
    return SD_RES::OK;
};
