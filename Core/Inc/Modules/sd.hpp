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

    template <size_t N>
    SDR_RES read(etl::string<N>& str)
    {
        char buf[N];  // must be char[] because f_read works that way

        UINT    bytes_read;
        FRESULT fres = f_read(&m_fil, buf, N, &bytes_read);

        if (fres) return SDR_RES::ERR;

        str.assign(buf);

        return SDR_RES::OK;
    }

    SDR_RES write(etl::string_view txt);

    SDR_RES seek(uint32_t offset);
    SDR_RES truncate();

    SDR_RES rename(etl::string_view old_path, etl::string_view new_path);

    uint32_t size() const;

    FIL* fil()
    {
        return &m_fil;
    }
    const FIL* fil() const
    {
        return &m_fil;
    }

    etl::string_view path() const
    {
        return m_path;
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

    SDR_RES mount();
    SDR_RES is_mounted() const;
    SDR_RES unmount();

    // File and directory Management
    SDFile* open_file(etl::string_view path, uint8_t mode);
    SDR_RES close_file(SDFile* file);

    SDR_RES exists(etl::string_view path);
    SDR_RES mkdir(etl::string_view path);

    SDR_RES list(etl::string_view dir_path, uint8_t page, etl::array<FILINFO, PAGE_SIZE>& out);
    SDR_RES delete_(etl::string_view path,
                    bool             recursive = false);  // both a file and directory can be passed

    uint64_t total_space() const;
    uint64_t free_space() const;
};
