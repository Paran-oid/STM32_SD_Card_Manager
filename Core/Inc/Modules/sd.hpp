#pragma once

extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/array.h>
#include <etl/memory.h>
#include <etl/string.h>

#include "utils.hpp"

constexpr uint8_t MAX_FILE_HANDLES = 4;
constexpr uint8_t PAGE_SIZE        = 16;  // 16 entities can be read at a time

class SDFile
{
   private:
    etl::string<100> m_path;
    FIL              m_fil;

   public:
    SDFile() = delete;

    SDFile(const etl::string<100>& path) : m_path {path}
    {
    }

    ~SDFile() = default;

    void write(uint8_t);

    template <size_t size>
    MP_RES read(etl::string<size>& str)
    {
        char buf[size];

        UINT    bytes_read;
        FRESULT fres = f_read(&m_fil, buf, size, &bytes_read);

        str.assign(buf);

        if (fres) return MP_RES::ERR;
        return MP_RES::OK;
    }

    MP_RES write(etl::string_view txt);

    MP_RES seek(uint32_t offset);
    MP_RES truncate();

    MP_RES rename(etl::string_view old_path, etl::string_view new_path);
    MP_RES delete_();

    uint32_t size() const;

    FIL* fil()
    {
        return &m_fil;
    }
    const FIL* fil() const
    {
        return &m_fil;
    }
};

class MicroSDReader
{
   private:
    SPI_HandleTypeDef& m_hspi;

    FATFS m_fs;

    etl::array<etl::unique_ptr<SDFile>, MAX_FILE_HANDLES> m_file_handles;

   public:
    enum class SDType
    {
        SDSC,
        SDHC,
        SDXC
    };

    MicroSDReader() = delete;

    MicroSDReader(SPI_HandleTypeDef& hspi) : m_hspi {hspi}
    {
    }

    ~MicroSDReader() = default;

    MP_RES mount();
    MP_RES is_mounted() const;
    MP_RES unmount();

    // File and directory Management
    SDFile* open_file(etl::string_view path, uint8_t mode);
    MP_RES  close_file(FIL* file);

    MP_RES exists(etl::string_view path);
    MP_RES mkdir(etl::string_view path);

    MP_RES list_files(etl::string_view dir_path, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out);

    uint64_t total_space() const;
    uint64_t free_space() const;
};
