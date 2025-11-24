#include "filesystem.hpp"

#include <etl/memory.h>

#include "ffconf.h"
#include "file.hpp"
#include "hal_init.hpp"

namespace stm_sd
{

namespace filesystem
{

// --------------------------------------------------
// Internal static state
// --------------------------------------------------

using file_ptr = etl::unique_ptr<file>;

static FATFS                                  m_fs;
static SPI_HandleTypeDef                      m_hspi;
static etl::array<file_ptr, MAX_FILE_HANDLES> m_file_handles;

void init(SPI_HandleTypeDef& hspi)
{
    m_hspi = hspi;
}

status mount()
{
    return map_fresult(f_mount(&m_fs, "", 1));
}

bool is_mounted()
{
    return m_fs.fs_type != 0;
}

status unmount()
{
    return map_fresult(f_mount(NULL, "", 1));
}

file* open(const string& path, uint8_t mode)
{
    FRESULT fres;
    for (auto& handle : m_file_handles)
    {
        if (!handle)
        {
            handle = etl::unique_ptr<file>(new file(path));
            if ((fres = f_open(handle->fil(), path.c_str(), mode)) == FR_OK) return handle.get();

            printf("%s\r\n", status_message(map_fresult(fres)));
            handle.reset();  // automatically frees the memory
        }
    }
    return nullptr;
}

status close(file* f)
{
    if (!f) return status::err;

    for (auto& handle : m_file_handles)
    {
        if (handle && handle.get() == f)
        {
            FRESULT fres = f_close(f->fil());
            if (fres != FR_OK) return map_fresult(fres);

            handle.reset();  // destroy file object
            return status::ok;
        }
    }

    return status::invalid_parameter;
}

status copy(const string& src, const string& dst, uint8_t modes)
{
    if (!exists(src.c_str())) return fail("src is empty");

    status  stat = status::ok;
    FRESULT fres = FR_OK;

    bool is_dst_dir = is_directory(dst);
    bool is_src_dir = is_directory(src);

    bool dst_exists = exists(dst.c_str());
    if (dst_exists && (modes & OVERWRITE) == 0)
        return fail("overwrite flag not enabled for overwriting the destination");
    if (dst_exists) remove(dst);

    if ((!is_src_dir && !is_dst_dir) || (!is_src_dir && is_dst_dir))
    {
        // copy(write) content of a file into another (or into a new/existing directory)

        PathData pdst = extract_path(dst);
        PathData psrc = extract_path(src);

        string cdst = dst;  // copy from dst

        file* fsrc = open(src, FA_OPEN_EXISTING | FA_READ);
        if (!fsrc) return status::err;

        if (!exists(pdst.folder)) mkdir(pdst.folder);
        if (pdst.filename.empty())
        {
            // folder directory, we get rid of the last / so that fatfs acknowledges the path
            cdst.resize(cdst.size() - 1);
        }

        file* fdst;
        if (is_directory(cdst))
        {
            // we are adding src file to a directory

            string ndst = pdst.folder;
            ndst.append("/").append(psrc.filename);
            fdst = open(ndst, FA_CREATE_ALWAYS | FA_WRITE);  // dst_folder/src_filename
        }
        else
        {
            // we are simply going to copy(write) data to another file
            fdst = open(cdst, FA_CREATE_ALWAYS | FA_WRITE);
        }

        if (!fdst) return status::err;

        etl::array<uint8_t, BLOCK_SIZE> rbuf = {};
        while (fsrc->read(rbuf))
        {
            if ((stat = fdst->write(rbuf)) != status::ok) return stat;
        }

        if ((stat = close(fdst)) != status::ok) return stat;
        if ((stat = close(fsrc)) != status::ok) return stat;
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

status remove(const string& s, bool recursive)
{
    string p;
    p.assign(s.c_str());

    if (p == "/" || p == "." || p.empty()) return status::err;

    FILINFO info;
    FRESULT fres;
    status  stat;
    DIR     dir;

    if (f_stat(p.c_str(), &info) != FR_OK) return status::err;

    // directory?
    if (info.fattrib & AM_DIR)
    {
        if (!recursive) return status::err;

        if (f_opendir(&dir, p.c_str()) != FR_OK) return status::err;

        for (;;)
        {
            if (f_readdir(&dir, &info) != FR_OK) return status::err;

            if (!info.fname[0]) break;

            if (!strcmp(info.fname, ".") || !strcmp(info.fname, "..")) continue;

            string full = p;
            if (full.back() != '/') full += '/';
            full += info.fname;

            if (info.fattrib & AM_DIR)
            {
                f_closedir(&dir);
                if ((stat = remove(full, true)) != status::ok) return stat;

                fres = f_opendir(&dir, p.c_str());
                if (fres != FR_OK) return map_fresult(fres);
            }
            else
            {
                if (f_unlink(full.c_str()) != FR_OK) return status::err;
            }
        }

        f_closedir(&dir);
    }

    return map_fresult(f_unlink(p.c_str()));
}

status rename(const string& old_name, const string& new_name)
{
    return map_fresult(f_rename(old_name.c_str(), new_name.c_str()));
}

status format(sdfs fmt)
{
    status stat;
    if (is_mounted())
    {
        if ((stat = unmount()) != status::ok) return stat;
    }

    DWORD work[_MAX_SS];
    if (f_mkfs("", (BYTE) fmt, 0, work, sizeof(work)) != FR_OK) return status::err;

    return mount();
}

bool exists(const string& p)
{
    return f_stat(p.c_str(), nullptr) == FR_OK;
}

status mkdir(const string& p)
{
    return map_fresult(f_mkdir(p.c_str()));
}

int8_t list(const string& dir_p, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out)
{
    DIR dir;
    if (f_opendir(&dir, dir_p.c_str()) != FR_OK) return -1;

    uint16_t file_read  = 0;
    uint16_t page_start = PAGE_SIZE * page;
    uint16_t page_end   = PAGE_SIZE * (page + 1);

    FILINFO  fno;
    uint32_t current_file = 0;

    for (;;)
    {
        if (f_readdir(&dir, &fno) != FR_OK) return -1;

        if (!fno.fname[0]) break;

        if (current_file >= page_start && current_file < page_end)
        {
            out[current_file - page_start] = fno;
            file_read++;
        }

        current_file++;
    }

    f_closedir(&dir);
    return file_read;
}

uint64_t total_space()
{
    FATFS* pfs = nullptr;

    if (f_getfree("", nullptr, &pfs) != FR_OK) return 0;

    return (uint64_t) (pfs->n_fatent - 2) * pfs->csize * 512;
}

uint64_t free_space()
{
    FATFS* pfs = nullptr;
    DWORD  free_clusters;

    if (f_getfree("", &free_clusters, &pfs) != FR_OK) return 0;

    return (uint64_t) free_clusters * pfs->csize * 512;
}

bool is_file(const string& p)
{
    FILINFO fi;
    if (f_stat(p.c_str(), &fi) != FR_OK) return false;

    return !(fi.fattrib & AM_DIR);
}

bool is_directory(const string& p)
{
    FILINFO fi;
    if (f_stat(p.c_str(), &fi) != FR_OK) return false;

    return (fi.fattrib & AM_DIR) != 0;
}

string label()
{
    TCHAR buf[MAX_LABEL_SIZE];
    if (f_getlabel("", buf, nullptr) != FR_OK) return "";

    string s;
    s.assign(buf);
    return s;
}

status set_label(const string& lab)
{
    return map_fresult(f_setlabel(lab.c_str()));
}

string cwd()
{
    string s;
    char   buf[MAX_DIR_SIZE];

    if (f_getcwd(buf, sizeof(buf)) != FR_OK) die("getcwd failed");

    s.assign(buf);
    return s;
}

status chdir(const string& p)
{
    return map_fresult(f_chdir(p.c_str()));
}

}  // namespace filesystem

}  // namespace stm_sd
