#include "hal_init.hpp"
#include "sd.hpp"

SDR_RES MicroSDHandler::mount()
{
    return f_mount(&m_fs, "", 1) == FR_OK ? SDR_RES::OK : SDR_RES::ERR;
}

SDR_RES MicroSDHandler::is_mounted() const
{
    return m_fs.fs_type != 0 ? SDR_RES::OK : SDR_RES::ERR;
}

SDR_RES MicroSDHandler::unmount()
{
    FRESULT res = f_mount(NULL, "", 1);
    if (res != FR_OK) return SDR_RES::ERR;
    return SDR_RES::OK;
}

// File and directory management
SDFile* MicroSDHandler::open_file(etl::string_view path, uint8_t mode)
{
    for (auto& handle : m_file_handles)
    {
        if (!handle)
        {
            handle = etl::unique_ptr<SDFile>(new SDFile(path.data()));
            if (f_open(handle->fil(), path.data(), mode) == FR_OK)
                return handle.get();
            else
                handle.reset();  // delete the object
        }
    }

    return nullptr;
}

SDR_RES MicroSDHandler::close_file(SDFile* file)
{
    if (!file) return SDR_RES::ERR;
    for (auto& handle : m_file_handles)
    {
        if (file->path() == handle->path())
        {
            if (f_close(file->fil()) != FR_OK) return SDR_RES::ERR;  // couldn't close the file
            handle.reset();
            return SDR_RES::OK;
        }
    }

    return SDR_RES::ERR;
}

SDR_RES MicroSDHandler::exists(etl::string_view path)
{
    return f_stat(path.data(), NULL) == FR_OK ? SDR_RES::OK : SDR_RES::ERR;
}

SDR_RES MicroSDHandler::mkdir(etl::string_view path)
{
    return f_mkdir(path.data()) == FR_OK ? SDR_RES::OK : SDR_RES::ERR;
}

SDR_RES MicroSDHandler::list(etl::string_view dir_path, uint8_t page,
                             etl::array<FILINFO, PAGE_SIZE>& out)
{
    (void) page;
    (void) out;

    DIR     dir;
    FRESULT fres = f_opendir(&dir, dir_path.data());

    uint16_t       file_count = 0;
    const uint16_t page_start = PAGE_SIZE * page;
    const uint16_t page_end   = PAGE_SIZE * (page + 1);

    if (fres != FR_OK) return SDR_RES::ERR;

    FILINFO fno;
    for (;;)
    {
        fres = f_readdir(&dir, &fno);
        if (fres != FR_OK) break;
        if (!fno.fname[0]) break;

        if (!((file_count > page_start) && (file_count < page_end)))
        {
            file_count++;
            continue;
        }

        out[file_count - page_start] = fno;
        file_count++;
    }

    fres = f_closedir(&dir);
    return fres == FR_OK ? SDR_RES::OK : SDR_RES::ERR;
}

SDR_RES MicroSDHandler::delete_(etl::string_view path, bool recursive)
{
    if (path != "/" || path == "." || path.empty())
    {
        return SDR_RES::ERR;
    }

    FILINFO fno;
    DIR     dir;

    FRESULT fres = f_stat(path.data(), &fno);

    if (fres != FR_OK) return SDR_RES::ERR;

    if (fno.fattrib & AM_DIR)
    {
        if (!recursive) return SDR_RES::ERR;

        fres = f_opendir(&dir, path.data());
        if (fres != FR_OK) return SDR_RES::ERR;

        for (;;)
        {
            fres = f_readdir(&dir, &fno);
            if (fres != FR_OK) return SDR_RES::ERR;
            if (fno.fname[0] == 0) break;  // error or end of directory

            if (etl::strcmp(fno.fname, ".") == 0 || etl::strcmp(fno.fname, "..") == 0) continue;

            etl::string<256> full_path;
            full_path.assign(path);
            if (full_path.back() != '/' && full_path.back() != '\\') full_path += '/';
            full_path += fno.fname;

            if ((fno.fattrib & AM_DIR))
            {
                fres = f_closedir(&dir);
                if (fres != FR_OK) return SDR_RES::ERR;

                if (this->delete_(full_path, recursive) != SDR_RES::OK) return SDR_RES::ERR;

                fres = f_opendir(&dir, path.data());
                if (fres != FR_OK) return SDR_RES::ERR;
            }
            else
            {
                fres = f_unlink(full_path.data());
                if (fres != FR_OK) return SDR_RES::ERR;
            }
        }

        fres = f_closedir(&dir);
    }

    fres = f_unlink(path.data());  // delete the file or empty directory

    return fres == FR_OK ? SDR_RES::OK : SDR_RES::ERR;
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

void MicroSDHandler::set_label(etl::string<MAX_LABEL_SIZE> new_label)
{
    if (f_setlabel(new_label.data()) != FR_OK)
    {
        m_label = "";
    }
    else
    {
        m_label = new_label;
    }
}
etl::string<MAX_LABEL_SIZE> MicroSDHandler::label()
{
    if (!m_label.empty())
    {
        return m_label;
    }

    char buf[MAX_LABEL_SIZE];
    if (f_getlabel("", buf, NULL) != FR_OK) return "";
    m_label.assign(buf);

    return m_label;
}
