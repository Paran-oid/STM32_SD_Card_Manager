#pragma once

extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/string.h>

#include "defines.hpp"

constexpr uint8_t MAX_FILE_HANDLES = 4;

class MicroSDReader
{
   private:
    SPI_HandleTypeDef& m_hspi;

    FATFS m_fs;

    etl::array<FIL, MAX_FILE_HANDLES> m_file_handles;

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
    FIL*   open_file(etl::string_view path, uint8_t mode);
    MP_RES close_file(FIL* file);

    MP_RES exists(etl::string_view path);
    MP_RES mkdir(etl::string_view path);
    MP_RES list_files(etl::string_view dir);

    uint64_t total_space() const;
    uint64_t free_space() const;
};

class SDFile
{
   private:
    MicroSDReader& m_sd_reader;
    FIL            m_fil;

   public:
    SDFile() = delete;

    SDFile(MicroSDReader& sd_reader) : m_sd_reader {sd_reader}
    {
    }

    ~SDFile() = default;

    void write(uint8_t);

    MP_RES write(etl::string_view txt);
    MP_RES append(etl::string_view txt);

    template <size_t size>
    MP_RES read(etl::string<size> txt);

    MP_RES seek(uint32_t offset);
    MP_RES truncate();

    uint32_t size() const;
    MP_RES   rename(etl::string_view old_path, etl::string_view new_path);
    MP_RES   remove();
};
