#include "sd_filesystem.hpp"

#include <etl/array.h>
#include <etl/memory.h>

#include "ffconf.h"
#include "printf.h"
#include "sd_file.hpp"

namespace stm_sd
{

namespace sd_filesystem
{

// --------------------------------------------------
// Internal static state
// --------------------------------------------------

using SDFilePtr = etl::unique_ptr<SDFile>;

static FATFS                                   m_fs;
static SPI_HandleTypeDef                       m_hspi;
static etl::array<SDFilePtr, MAX_FILE_HANDLES> m_fileHandles;

void init(SPI_HandleTypeDef& hspi)
{
    m_hspi = hspi;
}

Status mount()
{
    return mapFRESULT(f_mount(&m_fs, "", 1));
}

bool isMounted()
{
    return m_fs.fs_type != 0;
}

Status unmount()
{
    return mapFRESULT(f_mount(nullptr, "", 1));
}

SDFile* open(const string& path, uint8_t mode)
{
    FRESULT fres;
    for (auto& handle : m_fileHandles)
    {
        if (handle) continue;

        handle = etl::unique_ptr<SDFile>(new SDFile(path));
        if ((fres = f_open(handle->fil(), path.c_str(), mode)) == FR_OK) return handle.get();

        printf_("%s\r\n", statusMessageMap(mapFRESULT(fres)));
        handle.reset();  // automatically frees the memory
        return nullptr;
    }
    return nullptr;
}

Status close(SDFile* f)
{
    if (!f) return Status::ERR;

    for (auto& handle : m_fileHandles)
    {
        if (handle && handle.get() == f)
        {
            FRESULT fres = f_close(f->fil());
            if (fres != FR_OK) return mapFRESULT(fres);

            handle.reset();  // destroy file object
            return Status::OK;
        }
    }

    return Status::INVALID_PARAMETER;
}

Status copy(const string& src, const string& dst, uint8_t modes)
{
    if (!exists(src.c_str())) return fail("src is empty");

    Status  stat = Status::OK;
    FRESULT fres = FR_OK;

    bool isDstDir = isDirectory(dst);
    bool isSrcDir = isDirectory(src);

    bool dstExists = exists(dst.c_str());
    if (dstExists && (modes & OVERWRITE) == 0)
        return fail("overwrite flag not enabled for overwriting the destination");
    if (dstExists) remove(dst);

    if ((!isSrcDir && !isDstDir) || (!isSrcDir && isDstDir))
    {
        // copy(write) content of a file into another (or into a new/existing directory)

        PathData pdst = extractPath(dst);
        PathData psrc = extractPath(src);

        string cdst = dst;  // copy from dst

        SDFile* fsrc = open(src, FOPEN_EXISTING | FREAD);
        if (!fsrc) return Status::ERR;

        if (!exists(pdst.folder)) mkdir(pdst.folder);
        if (pdst.filename.empty())
        {
            // folder directory, we get rid of the last / so that fatfs acknowledges the path
            cdst.resize(cdst.size() - 1);
        }

        SDFile* fdst;
        if (isDirectory(cdst))
        {
            // we are adding src file to a directory

            string ndst = pdst.folder;
            ndst.append("/").append(psrc.filename);
            fdst = open(ndst, FCREATE_ALWAYS | FWRITE);  // dst_folder/src_filename
        }
        else
        {
            // we are simply going to copy(write) data to another file
            fdst = open(cdst, FCREATE_ALWAYS | FWRITE);
        }

        if (!fdst) return Status::ERR;

        etl::array<uint8_t, BLOCK_SIZE> rbuf = {};
        while (fsrc->read(rbuf))
        {
            if ((stat = fdst->write(rbuf)) != Status::OK) return stat;
        }

        if ((stat = close(fdst)) != Status::OK) return stat;
        if ((stat = close(fsrc)) != Status::OK) return stat;
    }

    else if (isSrcDir && isDstDir)
    {
        // put a directory inside another directory
        DIR srcDir;
        if ((fres = f_opendir(&srcDir, src.c_str())) != FR_OK) return mapFRESULT(fres);

        FILINFO fno;
        for (;;)
        {
            if ((fres = f_readdir(&srcDir, &fno)) != FR_OK) return mapFRESULT(fres);
            if (!fno.fname[0]) break;
            if (strcmp(fno.fname, ".") == 0 || strcmp(fno.fname, "..") == 0) continue;

            string srcChild = src;
            srcChild.append("/").append(fno.fname);

            string dstChild = dst;
            dstChild.append("/").append(fno.fname);

            if ((stat = copy(srcChild, dstChild, modes)) != Status::OK) return stat;
        }

        f_closedir(&srcDir);
    }
    else
    {
        return fail(
            "moving a directory into a file is unlogical");  // can't move a directory into a file
    }

    return Status::OK;
}

Status remove(const string& s, bool recursive)
{
    string p;
    p.assign(s.c_str());

    if (p == "/" || p == "." || p.empty()) return Status::ERR;

    FILINFO info;
    FRESULT fres;
    Status  stat;
    DIR     dir;

    if (f_stat(p.c_str(), &info) != FR_OK) return Status::ERR;

    // directory?
    if (info.fattrib & AM_DIR)
    {
        if (!recursive) return Status::ERR;

        if (f_opendir(&dir, p.c_str()) != FR_OK) return Status::ERR;

        for (;;)
        {
            if (f_readdir(&dir, &info) != FR_OK) return Status::ERR;

            if (!info.fname[0]) break;

            if (!strcmp(info.fname, ".") || !strcmp(info.fname, "..")) continue;

            string full = p;
            if (full.back() != '/') full += '/';
            full += info.fname;

            if (info.fattrib & AM_DIR)
            {
                f_closedir(&dir);
                if ((stat = remove(full, true)) != Status::OK) return stat;

                fres = f_opendir(&dir, p.c_str());
                if (fres != FR_OK) return mapFRESULT(fres);
            }
            else
            {
                if (f_unlink(full.c_str()) != FR_OK) return Status::ERR;
            }
        }

        f_closedir(&dir);
    }

    return mapFRESULT(f_unlink(p.c_str()));
}

Status rename(const string& oldName, const string& newName)
{
    return mapFRESULT(f_rename(oldName.c_str(), newName.c_str()));
}

Status format(SDFS fmt)
{
    //! FIXME

    Status stat;
    if (isMounted())
    {
        if ((stat = unmount()) != Status::OK) return stat;
    }

    DWORD work[_MAX_SS];
    if (f_mkfs("", (BYTE) fmt, 0, work, sizeof(work)) != FR_OK) return Status::ERR;

    return mount();
}

bool exists(const string& p)
{
    return f_stat(p.c_str(), nullptr) == FR_OK;
}

Status mkdir(const string& p)
{
    return mapFRESULT(f_mkdir(p.c_str()));
}

int8_t list(const string& dir_p, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out)
{
    DIR dir;
    if (f_opendir(&dir, dir_p.c_str()) != FR_OK) return -1;

    uint16_t filesRead = 0;
    uint16_t pageStart = PAGE_SIZE * page;
    uint16_t pageEnd   = PAGE_SIZE * (page + 1);

    FILINFO  fno;
    uint32_t currentFile = 0;

    for (;;)
    {
        if (f_readdir(&dir, &fno) != FR_OK) return -1;

        if (!fno.fname[0]) break;

        if (currentFile >= pageStart && currentFile < pageEnd)
        {
            out[currentFile - pageStart] = fno;
            filesRead++;
        }

        currentFile++;
    }

    f_closedir(&dir);
    return filesRead;
}

uint64_t totalSpace()
{
    FATFS* pfs = nullptr;

    if (f_getfree("", nullptr, &pfs) != FR_OK) return 0;

    return (uint64_t) (pfs->n_fatent - 2) * pfs->csize * 512;
}

uint64_t freeSpace()
{
    FATFS* pfs = nullptr;
    DWORD  freeClusters;

    if (f_getfree("", &freeClusters, &pfs) != FR_OK) return 0;

    return (uint64_t) freeClusters * pfs->csize * 512;
}

bool isFile(const string& p)
{
    FILINFO fi;
    if (f_stat(p.c_str(), &fi) != FR_OK) return false;

    return !(fi.fattrib & AM_DIR);
}

bool isDirectory(const string& p)
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

Status setLabel(const string& lab)
{
    return mapFRESULT(f_setlabel(lab.c_str()));
}

string cwd()
{
    string s;
    char   buf[MAX_DIR_SIZE];

    if (f_getcwd(buf, sizeof(buf)) != FR_OK) die("getcwd failed");

    s.assign(buf);
    return s;
}

Status chdir(const string& p)
{
    return mapFRESULT(f_chdir(p.c_str()));
}

}  // namespace sd_filesystem

}  // namespace stm_sd
