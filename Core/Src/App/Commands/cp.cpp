#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"

extern "C"
{
#include "fatfs.h"
}

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

constexpr uint8_t OVERWRITE = (1 << 1);
constexpr uint8_t RECURSIVE = (1 << 2);

extern CmdExec rm_exec;

// TODO: make a filesystem class and add this function there
StatusCode copy(const string& src, const string& dst, uint8_t modes)
{
    if (!fs::exists(src.c_str())) return StatusCode::ERR;

    bool is_dst_dir = fs::is_directory(dst);
    bool is_src_dir = fs::is_directory(src);

    bool dst_exists = fs::exists(dst.c_str());
    if (dst_exists && (modes & OVERWRITE) == 0) return StatusCode::ERR;
    if (dst_exists) rm_exec({dst});

    if (!is_src_dir && !is_dst_dir)
    {
        // copy(write) content of a file into another

        File* fdst = fs::open(dst, FA_CREATE_ALWAYS | FA_WRITE);
        if (!fdst) return StatusCode::ERR;
        File* fsrc = fs::open(src, FA_OPEN_EXISTING | FA_READ);
        if (!fsrc) return StatusCode::ERR;

        etl::string<BLOCK_SIZE> rbuf;
        while (fsrc->read(rbuf))
        {
            if (fdst->write(rbuf) != StatusCode::OK) return StatusCode::ERR;
        }

        if (fs::close(fdst) != StatusCode::OK) return StatusCode::ERR;
        if (fs::close(fsrc) != StatusCode::OK) return StatusCode::ERR;
    }
    else if (is_src_dir && !is_dst_dir)
    {
        // copy a file into a directory
        // TODO
    }
    else if (is_src_dir && is_dst_dir)
    {
        // put a directory inside another directory
        // TODO
    }
    else
    {
        return StatusCode::ERR;  // can't move a directory into a file
    }

    return StatusCode::OK;
}

CmdExec cp_exec = [](const etl::vector<string, ARGS_CAPACITY>& args)
{
    if (args.empty() || args.size() != 2) return StatusCode::ERR;

    const etl::string<SSIZE>& src = args[0];
    const etl::string<SSIZE>& dst = args[1];
    if (!fs::exists(src.c_str())) return StatusCode::ERR;

    if (fs::is_directory(dst.c_str()))
    {
        bool is_src_dir = fs::is_directory(src.c_str());

        uint8_t opts = OVERWRITE;
        if (is_src_dir) opts |= RECURSIVE;

        copy(src.c_str(), dst.c_str(), opts);  // copies into a directory the file or even
                                               // the directory in question
    }
    else
    {
        if (fs::is_directory(src.c_str()))
            return StatusCode::ERR;  // can't copy directory into a file???
        copy(src.c_str(), dst.c_str(), OVERWRITE);
    }
    return StatusCode::OK;
};

}  // namespace stm_sd
