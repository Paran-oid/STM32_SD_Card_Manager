#include "sd.hpp"

MP_RES MicroSDReader::mount()
{
    return f_mount(&m_fs, "", 1) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::is_mounted() const
{
    return m_fs.fs_type != 0 ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::unmount()
{
    FRESULT res = f_mount(NULL, "", 1);
    if (res != FR_OK) return MP_RES::ERR;
    return MP_RES::OK;
}

// File and directory management
SDFile* MicroSDReader::open_file(etl::string_view path, uint8_t mode)
{
    for (auto& file : m_file_handles)
    {
        if (!file)
        {
            file = etl::unique_ptr<SDFile>(new SDFile(path.data()));
            if (f_open(file->fil(), path.data(), mode) == FR_OK)
                return file.get();
            else
                file.reset();  // delete the object
        }
    }

    return nullptr;
}

MP_RES MicroSDReader::close_file(FIL* file)
{
    if (file)
    {
        return f_close(file) == FR_OK ? MP_RES::OK : MP_RES::ERR;
    }
    return MP_RES::ERR;
}

MP_RES MicroSDReader::exists(etl::string_view path)
{
    return f_stat(path.data(), NULL) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES MicroSDReader::mkdir(etl::string_view path)
{
    return f_mkdir(path.data()) == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

MP_RES list_files(etl::string_view dir_path, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out)
{
    (void) page;
    (void) out;

    DIR     dir;
    FRESULT fres = f_opendir(&dir, dir_path.data());

    if (fres != FR_OK) return MP_RES::ERR;

    // TODO: make sure the array passed is filled after this function

    FILINFO fno;
    for (;;)
    {
        fres = f_readdir(&dir, &fno);
        if (fres != FR_OK) break;
        if (!fno.fname[0]) break;

        if (fno.fattrib & AM_DIR)
        {
            log("  <DIR>  ");
            log(fno.fname);
            log("\n");
        }
        else
        {
            log("  <FILE>  ");
            log(fno.fname);
            log("\n");
        }
    }

    fres = f_closedir(&dir);
    return fres == FR_OK ? MP_RES::OK : MP_RES::ERR;
}

uint64_t MicroSDReader::total_space() const
{
    FATFS* pfs;

    FRESULT fres = f_getfree(NULL, NULL, &pfs);
    if (fres != FR_OK) return 0;

    return (pfs->n_fatent - 2) * pfs->csize;
}

uint64_t MicroSDReader::free_space() const
{
    DWORD  free_clusters;
    FATFS* pfs;

    FRESULT fres = f_getfree(NULL, &free_clusters, &pfs);
    if (fres != FR_OK) return 0;

    return free_clusters * pfs->csize;
}
