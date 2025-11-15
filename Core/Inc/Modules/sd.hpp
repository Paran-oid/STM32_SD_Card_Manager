#pragma once

extern "C"
{
#include "fatfs.h"
#include "main.h"
}

#include "etl/array.h"
#include "etl/memory.h"
#include "etl/string.h"
#include "utils.hpp"

constexpr uint8_t MAX_FILE_HANDLES = 4;
constexpr uint8_t PAGE_SIZE        = 4;  // 16 entities can be read at a time
constexpr uint8_t MAX_LABEL_SIZE   = 32;

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
    SD_RES read(etl::string<N>& str)
    {
        char buf[N - 1];  // must be char[] because f_read works that way
        UINT bytes_read = 0;

        FRESULT fres = f_read(&m_fil, buf, N - 1, &bytes_read);
        if (fres != FR_OK || bytes_read == 0) return SD_RES::ERR;

        str.assign(buf);
        str[bytes_read] = '\0';  // content of files don't null terminate
        return SD_RES::OK;
    }

    SD_RES write(etl::string_view txt);

    SD_RES seek(uint32_t offset);
    SD_RES truncate();

    SD_RES rename(etl::string_view old_path, etl::string_view new_path);

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

class MicroSDHandler
{
   private:
    SPI_HandleTypeDef& m_hspi;

    FATFS m_fs;

    etl::array<etl::unique_ptr<SDFile>, MAX_FILE_HANDLES> m_file_handles;

    etl::string<SSIZE>          m_cwd   = "/";
    etl::string<MAX_LABEL_SIZE> m_label = "";

   public:
    enum class SDType
    {
        SDSC,
        SDHC,
        SDXC
    };

    MicroSDHandler() = delete;

    MicroSDHandler(SPI_HandleTypeDef& hspi) : m_hspi {hspi}
    {
    }

    ~MicroSDHandler() = default;

    SD_RES mount();
    SD_RES is_mounted() const;
    SD_RES unmount();

    // File and directory Management
    SDFile* open_file(const etl::string<SSIZE>& path, uint8_t mode);
    SD_RES  close_file(SDFile* file);

    SD_RES exists(const etl::string<SSIZE>& path);
    SD_RES mkdir(const etl::string<SSIZE>& path);

    SD_RES list(const etl::string<SSIZE>& dir_path, uint8_t page,
                etl::array<FILINFO, PAGE_SIZE>& out);
    SD_RES delete_(const etl::string<SSIZE>& path,
                   bool recursive = false);  // both a file and directory can be passed

    uint64_t total_space() const;
    uint64_t free_space() const;

    void                        set_label(const etl::string<MAX_LABEL_SIZE>& new_label);
    etl::string<MAX_LABEL_SIZE> label();

    void set_cwd(const etl::string<SSIZE>& dir)
    {
        m_cwd = dir;
    }
    etl::string<SSIZE> cwd()
    {
        return m_cwd;
    }
};
