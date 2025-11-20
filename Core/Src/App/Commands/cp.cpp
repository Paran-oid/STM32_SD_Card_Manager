#include "command_handler.hpp"
#include "etl/string.h"
#include "etl/vector.h"
#include "hal_init.hpp"
#include "utils.hpp"
#include "filesystem.hpp"


extern "C"
{
#include "fatfs.h"
}

namespace fs = stm_sd::filesystem;

namespace stm_sd
{

extern cmd_exec rm_exec;

// TODO: start using path and string types accordingly

// TODO: make a filesystem class and add this function there
status copy(const string& src, const string& dst, uint8_t modes)
{
    if (!fs::exists(src.c_str())) return fail("src is empty");

    status  stat;
    FRESULT fres;

    bool is_dst_dir = fs::is_directory(dst);
    bool is_src_dir = fs::is_directory(src);

    bool dst_exists = fs::exists(dst.c_str());
    if (dst_exists && (modes & OVERWRITE) == 0)
        return fail("overwrite flag not enabled for overwriting the destination");
    if (dst_exists) rm_exec({dst});

    if ((!is_src_dir && !is_dst_dir) || (!is_src_dir && is_dst_dir))
    {
        // copy(write) content of a file into another (or into a new/existing directory)

        PathData pdst = extract_path(dst);
        PathData psrc = extract_path(src);

        string cdst = dst;  // copy from dst

        file* fsrc = fs::open(src, FA_OPEN_EXISTING | FA_READ);
        if (!fsrc) return status::err;

        if (!fs::exists(pdst.folder)) fs::mkdir(pdst.folder);
        if (pdst.filename.empty())
        {
            // folder directory, we get rid of the last / so that fatfs acknowledges the path
            cdst.resize(cdst.size() - 1);
        }

        file* fdst;
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

        if (!fdst) return status::err;

        etl::array<uint8_t, BLOCK_SIZE> rbuf = {};
        while (fsrc->read(rbuf))
        {
            if ((stat = fdst->write(rbuf)) != status::ok) return stat;
        }

        if ((stat = fs::close(fdst)) != status::ok) return stat;
        if ((stat = fs::close(fsrc)) != status::ok) return stat;
    }

    else if (is_src_dir && is_dst_dir)
    {
        // put a directory inside another directory
        DIR src_dir;
        if ((fres = f_opendir(&src_dir, src.c_str())) != FR_OK) return map_fresult(fres);

        FILINFO fno;
        for (;;)
        {
            if ((fres = f_readdir(&src_dir, &fno)) != FR_OK) return map_fresult(fres);
            if (!fno.fname[0]) break;
            if (strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0) continue;

            string src_child = src;
            src_child.append("/").append(fno.fname);

            string dst_child = dst;
            dst_child.append("/").append(fno.fname);

            if ((stat = copy(src_child, dst_child, modes)) != status::ok) return stat;
        }

        f_closedir(&src_dir);
    }
    else
    {
        return fail(
            "moving a directory into a file is unlogical");  // can't move a directory into a file
    }

    return status::ok;
}

cmd_exec cp_exec = [](const cmd_args& args)
{
    if (args.size() != 2) return fail("2 arguments must be passed");

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
    if (!fs::exists(src.c_str())) return status::no_file;

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
            return fail(
                "recursive flag needed to copy directories");  // can't copy directory into a file
                                                               // and/or recursive flag is needed
        copy(src.c_str(), dst.c_str(), OVERWRITE);
    }
    return status::ok;
};

}  // namespace stm_sd
