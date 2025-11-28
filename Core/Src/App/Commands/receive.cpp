#include "command_handler.hpp"
#include "etl/to_arithmetic.h"
#include "filesystem.hpp"
#include "hal_init.hpp"
#include "scal/uart.hpp"
#include "status.hpp"
#include "utils.hpp"

namespace stm_sd
{

namespace fs = stm_sd::filesystem;

#define DEFAULT_RECEIVE_TIMEOUT_SECONDS 5

CmdExec receiveExec = [](const CmdArgs& args)
{
    if (args.empty()) return Status::INVALID_PARAMETER;

    uint32_t timeoutMS = 0;  // whenever we multiply by 1000 it is to turn it from mseconds into
                             // seconds
    string path;
    for (auto it = args.begin(); it != args.end(); ++it)
    {
        // check if timeout flag exists
        const auto& str = *it;  // content of iterator
        if (str == "-s")
        {
            auto next_it = etl::next(it);
            if (next_it == args.end()) continue;

            const auto&               nextStr = *next_it;
            etl::to_arithmetic_result cres    = etl::to_arithmetic<uint32_t>(nextStr);
            if (cres.error() != etl::to_arithmetic_status::Valid) return Status::INVALID_PARAMETER;

            timeoutMS = etl::to_arithmetic<uint32_t>(nextStr).value() * 1000;
        }
        else if (str == "-o")
        {
            auto next_it = etl::next(it);
            if (next_it == args.end()) continue;

            path = formatStr(*next_it);
        }
    }

    if (timeoutMS == 0) timeoutMS = DEFAULT_RECEIVE_TIMEOUT_SECONDS * 1000;
    if (path.empty() || !isValidPath(path)) return fail("no output file was passed...");

    // open the file to be writing to
    File* f = fs::open(path, FCREATE_NEW | FWRITE);
    if (!f)
    {
        // TODO: make sure to format
        return fail("unexpected error...");
    }

    printf_(CLEAR_CMD);
    printf_("### WAITING FOR DATA TO BE SENT! ###\r\n");

    uint8_t  c;
    uint64_t bytesWritten = 0;
    do
    {
        auto res = uart2.receive(c, timeoutMS);
        if (res != HAL_OK)
        {
            fs::close(f);
            return fail("didn't receive anything...");
        }

        bytesWritten++;
        f->write((const char*) &c);  // TODO: make this work ffs
    } while (true);

    printf_("successfully written %llu bytes to %s\r\n", bytesWritten, path.c_str());
    fs::close(f);

    return Status::OK;
};

}  // namespace stm_sd
