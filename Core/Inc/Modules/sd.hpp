#pragma once

extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include <etl/string.h>

#include "defines.hpp"

class MicroSDReader
{
   private:
    SPI_HandleTypeDef& m_hspi;

    FATFS m_fs;
    FIL   m_fil;

    bool m_is_mounted  = false;
    bool m_file_opened = false;

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
    MP_RES open_file(etl::string_view path, uint8_t mode);
    MP_RES close_file();

    MP_RES write_file(etl::string_view path, etl::string_view data);

    template <size_t N>
    MP_RES read_file(etl::string_view path, etl::string<N> data);
    MP_RES append_file(etl::string_view path, etl::string_view data);

    MP_RES seek(uint32_t offset);
    MP_RES truncate(etl::string_view path);

    uint32_t file_size(etl::string_view path) const;
    MP_RES   rename_file(etl::string_view old_path, etl::string_view new_path);
    MP_RES   delete_file(etl::string_view path);

    MP_RES exists(etl::string_view path);
    MP_RES mkdir(etl::string_view path);
    MP_RES list_files(etl::string_view dir);

    uint64_t total_space() const;
    uint64_t free_space() const;
};
