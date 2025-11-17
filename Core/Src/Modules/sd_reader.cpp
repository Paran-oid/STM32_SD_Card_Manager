#include "hal_init.hpp"
#include "sd.hpp"

SD_RES MicroSDHandler::mount()
{
    return f_mount(&m_fs, "", 1) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

SD_RES MicroSDHandler::is_mounted() const
{
    return m_fs.fs_type != 0 ? SD_RES::OK : SD_RES::ERR;
}

SD_RES MicroSDHandler::unmount()
{
    FRESULT res = f_mount(NULL, "", 1);
    if (res != FR_OK) return SD_RES::ERR;
    return SD_RES::OK;
}

// File and directory management
SDFile* MicroSDHandler::open_file(const estring& path, uint8_t mode)
{
    for (auto& handle : m_file_handles)
    {
        if (!handle)
        {
            handle = etl::unique_ptr<SDFile>(new SDFile(path.data()));
            if (f_open(handle->fil(), path.c_str(), mode) == FR_OK)
                return handle.get();
            else
                handle.reset();  // delete the object
        }
    }

    return nullptr;
}

SD_RES MicroSDHandler::close_file(SDFile* file)
{
    if (!file) return SD_RES::ERR;
    for (auto& handle : m_file_handles)
    {
        if (file->path() == handle->path())
        {
            if (f_close(file->fil()) != FR_OK) return SD_RES::ERR;  // couldn't close the file
            handle.reset();
            return SD_RES::OK;
        }
    }

    return SD_RES::ERR;
}

bool MicroSDHandler::exists(const estring& path)
{
    return f_stat(path.c_str(), NULL) == FR_OK ? true : false;
}

SD_RES MicroSDHandler::mkdir(const estring& path)
{
    return f_mkdir(path.c_str()) == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

SD_RES MicroSDHandler::list(const estring& dir_path, uint8_t page,
                            etl::array<FILINFO, PAGE_SIZE>& out)
{
    DIR dir;
    if (f_opendir(&dir, dir_path.c_str()) != FR_OK) return SD_RES::ERR;

    uint16_t       file_count = 0;
    const uint16_t page_start = PAGE_SIZE * page;
    const uint16_t page_end   = PAGE_SIZE * static_cast<uint16_t>((page + 1));

    FILINFO fno;
    bool    filled = false;
    for (;;)
    {
        if (f_readdir(&dir, &fno) != FR_OK) return SD_RES::ERR;
        if (!fno.fname[0]) break;

        if ((file_count < page_start) || (file_count >= page_end))
        {
            file_count++;
            continue;
        }

        filled                       = true;
        out[file_count - page_start] = fno;
        file_count++;
    }

    if (f_closedir(&dir) != FR_OK) return SD_RES::ERR;
    return filled ? SD_RES::OK : SD_RES::ERR;
}

SD_RES MicroSDHandler::delete_(const estring& path, bool recursive)
{
    if (path == "/" || path == "." || path.empty())
    {
        return SD_RES::ERR;
    }

    FILINFO fno;
    DIR     dir;

    FRESULT fres = f_stat(path.c_str(), &fno);
    if (fres != FR_OK) return SD_RES::ERR;

    if (fno.fattrib & AM_DIR)
    {
        if (!recursive) return SD_RES::ERR;

        fres = f_opendir(&dir, path.c_str());
        if (fres != FR_OK) return SD_RES::ERR;

        for (;;)
        {
            fres = f_readdir(&dir, &fno);
            if (fres != FR_OK) return SD_RES::ERR;
            if (fno.fname[0] == 0) break;  // error or end of directory

            if (etl::strcmp(fno.fname, ".") == 0 || etl::strcmp(fno.fname, "..") == 0) continue;

            etl::string<256> full_path;
            full_path.assign(path);
            if (full_path.back() != '/' && full_path.back() != '\\') full_path += '/';
            full_path += fno.fname;

            if ((fno.fattrib & AM_DIR))
            {
                fres = f_closedir(&dir);
                if (fres != FR_OK) return SD_RES::ERR;

                if (this->delete_(full_path, recursive) != SD_RES::OK) return SD_RES::ERR;

                fres = f_opendir(&dir, path.c_str());
                if (fres != FR_OK) return SD_RES::ERR;
            }
            else
            {
                fres = f_unlink(full_path.c_str());
                if (fres != FR_OK) return SD_RES::ERR;
            }
        }

        fres = f_closedir(&dir);
    }

    fres = f_unlink(path.c_str());  // delete the file or empty directory

    return fres == FR_OK ? SD_RES::OK : SD_RES::ERR;
}

uint64_t MicroSDHandler::total_space() const
{
    FATFS* pfs;

    FRESULT fres = f_getfree(NULL, NULL, &pfs);
    if (fres != FR_OK) return 0;

    return (pfs->n_fatent - 2) * pfs->csize;
}

uint64_t MicroSDHandler::free_space() const
{
    DWORD  free_clusters;
    FATFS* pfs;

    FRESULT fres = f_getfree(NULL, &free_clusters, &pfs);
    if (fres != FR_OK) return 0;

    return free_clusters * pfs->csize;
}

bool MicroSDHandler::is_file(const estring& path)
{
    FILINFO fno;
    FRESULT res = f_stat(path.c_str(), &fno);
    if (res != FR_OK) return false;

    return !(fno.fattrib & AM_DIR);
}

bool MicroSDHandler::is_directory(const estring& path)
{
    FILINFO fno;
    FRESULT res = f_stat(path.c_str(), &fno);
    if (res != FR_OK) return false;

    return (fno.fattrib & AM_DIR) != 0;
}

SD_RES MicroSDHandler::set_label(const etl::string<MAX_LABEL_SIZE>& new_label)
{
    if (f_setlabel(new_label.c_str()) != FR_OK) return SD_RES::ERR;
    return SD_RES::OK;
}
etl::string<MAX_LABEL_SIZE> MicroSDHandler::label()
{
    estring res;
    TCHAR   buf[MAX_LABEL_SIZE];

    if (f_getlabel("", buf, nullptr) != FR_OK) return "";
    res.assign(buf);

    return res;
}

SD_RES MicroSDHandler::chdir(const estring& dir)
{
    if (f_chdir(dir.data()) != FR_OK) return SD_RES::ERR;
    return SD_RES::OK;
}

estring MicroSDHandler::cwd()
{
    estring res;
    char    buf[MAX_WD_SIZE];

    if (f_getcwd(buf, sizeof(buf)) != FR_OK) die("error trying to get cwd\r\n");
    res.assign(buf);

    return res;
}
