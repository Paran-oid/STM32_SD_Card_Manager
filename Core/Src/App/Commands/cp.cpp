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

// TODO: start using path and string types accordingly

// TODO: make a filesystem class and add this function there
StatusCode copy(const string& src, const string& dst, uint8_t modes)
{
    if (!fs::exists(src.c_str())) return StatusCode::ERR;

    bool is_dst_dir = fs::is_directory(dst);
    bool is_src_dir = fs::is_directory(src);

    bool dst_exists = fs::exists(dst.c_str());
    if (dst_exists && (modes & OVERWRITE) == 0) return StatusCode::ERR;
    if (dst_exists) rm_exec({dst});

    if ((!is_src_dir && !is_dst_dir) || (!is_src_dir && is_dst_dir))
    {
        // copy(write) content of a file into another (or into a new/existing directory)

        PathData pdst = extract_path(dst);
        PathData psrc = extract_path(src);

        string cdst = dst;  // copy from dst

        File* fsrc = fs::open(src, FA_OPEN_EXISTING | FA_READ);
        if (!fsrc) return StatusCode::ERR;

        if (!fs::exists(pdst.folder)) fs::mkdir(pdst.folder);
        if (pdst.filename.empty())
        {
            // folder directory, we get rid of the last / so that fatfs acknowledges the path
            cdst.resize(cdst.size() - 1);
        }

        File* fdst;
        if (fs::is_directory(cdst))
        {
            // we are adding src file to a directory

            string ndst = pdst.folder;
            ndst.append("/").append(psrc.filename);
            fdst = fs::open(ndst, FA_CREATE_ALWAYS | FA_WRITE);  // dst_folder/src_filename
        }
        else
        {
            // we are simply going to copy(write) data to another file
            fdst = fs::open(cdst, FA_CREATE_ALWAYS | FA_WRITE);
        }

        if (!fdst) return StatusCode::ERR;

        etl::array<uint8_t, BLOCK_SIZE> rbuf = {};
        while (fsrc->read(rbuf))
        {
            if (fdst->write(rbuf) != StatusCode::OK) return StatusCode::ERR;
        }

        if (fs::close(fdst) != StatusCode::OK) return StatusCode::ERR;
        if (fs::close(fsrc) != StatusCode::OK) return StatusCode::ERR;
    }

    else if (is_src_dir && is_dst_dir)
    {
        // put a directory inside another directory
        DIR src_dir;
        if (f_opendir(&src_dir, src.c_str()) != FR_OK) return StatusCode::ERR;

        FILINFO fno;
        for (;;)
        {
            if (f_readdir(&src_dir, &fno) != FR_OK) return StatusCode::ERR;
            if (!fno.fname[0]) break;
            if (strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0) continue;

            string src_child = src;
            src_child.append("/").append(fno.fname);

            string dst_child = dst;
            dst_child.append("/").append(fno.fname);

            // TODO: fix the problem here of child getting nullptr returned (files), maybe cuz lots
            // ;of file handles??
            if (copy(src_child, dst_child, modes) != StatusCode::OK) return StatusCode::ERR;

            // TODO: make it copy directories inside src too
        }

        f_closedir(&src_dir);
    }
    else
    {
        return StatusCode::ERR;  // can't move a directory into a file
    }

    return StatusCode::OK;
}

CmdExec cp_exec = [](const CmdArgs& args)
{
    if (args.empty() || args.size() != 2) return StatusCode::ERR;

    bool is_recursive = false;
    for (const auto& arg : args)
    {
        if (is_flag(arg))
        {
            for (auto it = arg.begin() + 1; it != arg.end(); it++)
            {
                char c = *it;
                if (c == 'r') is_recursive = true;
            }
        }
    }

    const string& src = args[0];
    const string& dst = args[1];
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
        if (fs::is_directory(src.c_str()) || !is_recursive)
            return StatusCode::ERR;  // can't copy directory into a file and/or recursive flag is
                                     // needed
        copy(src.c_str(), dst.c_str(), OVERWRITE);
    }
    return StatusCode::OK;
};

}  // namespace stm_sd
