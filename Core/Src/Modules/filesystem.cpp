#include "etl/memory.h"
#include "ffconf.h"
#include "hal_init.hpp"
#include "sd.hpp"

namespace stm_sd
{

namespace filesystem
{

// --------------------------------------------------
// Internal static state
// --------------------------------------------------

using FilePtr = etl::unique_ptr<File>;

static FATFS                                 m_fs;
static SPI_HandleTypeDef                     m_hspi;
static etl::array<FilePtr, MAX_FILE_HANDLES> m_file_handles;

void init(SPI_HandleTypeDef& hspi)
{
    m_hspi = hspi;
}

StatusCode mount()
{
    return (f_mount(&m_fs, "", 1) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

bool is_mounted()
{
    return m_fs.fs_type != 0;
}

StatusCode unmount()
{
    return (f_mount(NULL, "", 1) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

File* open(const string& path, uint8_t mode)
{
    for (auto& h : m_file_handles)
    {
        if (!h)
        {
            h = etl::unique_ptr<File>(new File(path));
            if (f_open(h->fil(), path.c_str(), mode) == FR_OK) return h.get();

            h.reset();
        }
    }
    return nullptr;
}

StatusCode close(File* file)
{
    if (!file) return StatusCode::ERR;

    for (auto& h : m_file_handles)
    {
        if (h && file->path() == h->path())
        {
            if (f_close(file->fil()) != FR_OK) return StatusCode::ERR;

            h.reset();
            return StatusCode::OK;
        }
    }

    return StatusCode::ERR;
}

StatusCode remove(const string& s, bool recursive)
{
    path p;
    p.assign(s.c_str());

    if (p == "/" || p == "." || p.empty()) return StatusCode::ERR;

    FILINFO info;
    DIR     dir;

    if (f_stat(p.c_str(), &info) != FR_OK) return StatusCode::ERR;

    // directory?
    if (info.fattrib & AM_DIR)
    {
        if (!recursive) return StatusCode::ERR;

        if (f_opendir(&dir, p.c_str()) != FR_OK) return StatusCode::ERR;

        for (;;)
        {
            if (f_readdir(&dir, &info) != FR_OK) return StatusCode::ERR;

            if (!info.fname[0]) break;

            if (!strcmp(info.fname, ".") || !strcmp(info.fname, "..")) continue;

            path full = p;
            if (full.back() != '/') full += '/';
            full += info.fname;

            if (info.fattrib & AM_DIR)
            {
                f_closedir(&dir);
                if (remove(full, true) != StatusCode::OK) return StatusCode::ERR;

                if (f_opendir(&dir, p.c_str()) != FR_OK) return StatusCode::ERR;
            }
            else
            {
                if (f_unlink(full.c_str()) != FR_OK) return StatusCode::ERR;
            }
        }

        f_closedir(&dir);
    }

    return (f_unlink(p.c_str()) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

StatusCode format(SDFileSystem fmt)
{
    if (is_mounted())
    {
        if (unmount() != StatusCode::OK) return StatusCode::ERR;
    }

    DWORD work[_MAX_SS];
    if (f_mkfs("", (BYTE) fmt, 0, work, sizeof(work)) != FR_OK) return StatusCode::ERR;

    return mount();
}

bool exists(const string& p)
{
    return f_stat(p.c_str(), nullptr) == FR_OK;
}

StatusCode mkdir(const path& p)
{
    return (f_mkdir(p.c_str()) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

int8_t list(const path& dir_p, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out)
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

StatusCode delete_(const path& p, bool recursive)
{
    return remove(p.c_str(), recursive);
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

bool is_file(const path& p)
{
    FILINFO fi;
    if (f_stat(p.c_str(), &fi) != FR_OK) return false;

    return !(fi.fattrib & AM_DIR);
}

bool is_directory(const path& p)
{
    FILINFO fi;
    if (f_stat(p.c_str(), &fi) != FR_OK) return false;

    return (fi.fattrib & AM_DIR) != 0;
}

etl::string<MAX_LABEL_SIZE> label()
{
    TCHAR buf[MAX_LABEL_SIZE];
    if (f_getlabel("", buf, nullptr) != FR_OK) return "";

    etl::string<MAX_LABEL_SIZE> s;
    s.assign(buf);
    return s;
}

StatusCode set_label(const etl::string<MAX_LABEL_SIZE>& l)
{
    return (f_setlabel(l.c_str()) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

string cwd()
{
    string s;
    char   buf[MAX_DIR_SIZE];

    if (f_getcwd(buf, sizeof(buf)) != FR_OK) die("getcwd failed");

    s.assign(buf);
    return s;
}

StatusCode chdir(const string& p)
{
    return (f_chdir(p.c_str()) == FR_OK) ? StatusCode::OK : StatusCode::ERR;
}

}  // namespace filesystem

}  // namespace stm_sd
